/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */

package Bittorrent;

import java.awt.Dimension;
import java.awt.FlowLayout;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JProgressBar;
import javax.swing.SwingConstants;
import javax.swing.SwingUtilities;

/**
 *
 * @author karladbenitezquiroz
 */
public class servidorTorrent extends JFrame{
private JProgressBar progressBar;
private JLabel statusLabel;
    
    public servidorTorrent(int fileSize) {
        setTitle("Barra de Descarga");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(300, 150);
        setLayout(new FlowLayout());

        if (fileSize > 10000) {
            progressBar = new JProgressBar();
            progressBar.setPreferredSize(new Dimension(250, 30));
            progressBar.setStringPainted(true);

            statusLabel = new JLabel("Descargando...");
            statusLabel.setHorizontalAlignment(SwingConstants.CENTER);

            add(progressBar);
            add(statusLabel);

            // Simulación de la descarga del archivo
            new Thread(new DownloadTask(progressBar, statusLabel)).start();
        } 
    }
    
    static class verificarArchivo extends Thread{
    //Defenimos variables para controlar los tiempos
    float initTime=5000,tiempInicial=0,tiempoFInal=0,tiempoTotal=0;
    
    int cliente;
    Socket miSocket=null;
    ObjectInputStream entrada;
    ObjectOutputStream salida;
    DataInputStream entradaDatos;
    DataOutputStream salidaDatos;
    File miArchivo=null;
    
    verificarArchivo(Socket socket, int cliente){
        this.miSocket = socket;
        this.cliente = cliente;
    }
    //Funcion para verificar si existe el archivo dentro de la biblioteca
    boolean existe(String archivoivo){
        miArchivo = new File("biblioteca/"+archivoivo);
        Boolean res;
        res = miArchivo.exists();
        return res;
    }
     
    public void run(){
    try{
    //Construimos la entrada y salida de los datos hacia el cliente
    entradaDatos = new DataInputStream(miSocket.getInputStream());
    salidaDatos = new DataOutputStream(miSocket.getOutputStream());
    String archivo = entradaDatos.readUTF();
    //Aqui se verifica si el archivo esta en la biblioteca con la funcion existe
    if(existe(archivo)==true){
        tiempInicial=(System.currentTimeMillis()-this.initTime);
        System.out.println("Enviando archivo torrent al peer con ID: "+cliente+" \n En un tiempo de: "+(System.currentTimeMillis()-this.initTime)+"ms");
        salidaDatos.writeBoolean(true);
        salidaDatos.writeUTF("Archivo: " + archivo + " existente en la biblioteca del tracker");
        salidaDatos.writeUTF("Archivo con tamaño de: " + (miArchivo.length() / 1024) + " KB   Archivo" + miArchivo.getName());
       // salidaDatos.writeInt((int) miArchivo.length());
        long longitudArchivo = miArchivo.length();
        salidaDatos.writeInt((int) longitudArchivo);

        salidaDatos.writeUTF(archivo);
        System.out.println("\nEnviando archivo: " + archivo + " a la direccion: " + miSocket.getInetAddress());
        FileInputStream misBytes = new FileInputStream(miArchivo);
        BufferedInputStream lectura = new BufferedInputStream(misBytes);//obtain input bytes from a file in a file system
                    
                    BufferedOutputStream salida = new BufferedOutputStream(miSocket.getOutputStream()); 
                    byte[] arreglo = new byte[(int) miArchivo.length()];
                    lectura.read(arreglo);

                    for (int i = 0; i < arreglo.length; i++) {
                        salida.write(arreglo[i]);
                    }
                    tiempoFInal=(System.currentTimeMillis() - this.initTime);
                    tiempoTotal=tiempoFInal-tiempInicial;
                    
                     SwingUtilities.invokeLater(new Runnable() {
                         @Override
                     public void run() {
                     new DownloadProgressBar((int) longitudArchivo);
                     }
                    });
                     
                    System.out.println("\nArchivo Enviado a peer: " + cliente);

                    System.out.println("\nEl tracker termino el envio al cliente: " + cliente + " en:  "
                            + tiempoTotal + " ms");
                    System.out.println("\nTiempo del peer: "+cliente +": ("+(System.currentTimeMillis() + this.initTime)+") ms");

                    salida.flush();
                    salida.flush();
               //     salida.close();
                 //   misBytes.close();
                }

                if (existe(archivo) == false) {//Si el archivo no existe en la biblioteca
                    salidaDatos.writeBoolean(false);
                    salidaDatos.writeUTF("\nEl archivo: " + archivo + " no se encuentra en la biblioteca del servidor");
                    //System.out.println("Respuesta enviada al cliente");
                }

    
    
    }catch(Exception ex){
    System.out.println("Error: "+ex.getMessage()+" para el cliente con id: "+cliente);
    }finally {
                try {
                    if (salida != null) {
                        salida.close();
                    }
                    if (entrada != null) {
                        entrada.close();
                    }
                    if (miSocket != null) {
                        miSocket.close();
                    }
                    System.out.println("\nEnvio exitoso al peer: "+cliente);

                } catch (Exception e) {
                    System.out.println("\nMensaje error:  "+e.getMessage());
                }
            }
    }
    }
    
    private static class DownloadTask implements Runnable {
        private JProgressBar progressBar;
        private JLabel statusLabel;

        public DownloadTask(JProgressBar progressBar, JLabel statusLabel) {
            this.progressBar = progressBar;
            this.statusLabel = statusLabel;
        }

        @Override
        public void run() {
            int progress = 0;
            while (progress <= 100) {
                try {
                    Thread.sleep(300);  // Simular tiempo de descarga
                } catch (InterruptedException ex) {
                    ex.printStackTrace();
                }
                progressBar.setValue(progress);
                progress += 10;
            }

            try {
                Thread.sleep(4000);  // Esperar 5 segundos
            } catch (InterruptedException ex) {
                ex.printStackTrace();
            }

            statusLabel.setText("Descarga completada");

            try {
                Thread.sleep(2000);  // Esperar 5 segundos antes de cerrar la ventana
            } catch (InterruptedException ex) {
                ex.printStackTrace();
            }

            SwingUtilities.invokeLater(new Runnable() {
                @Override
                public void run() {
                    JFrame frame = (JFrame) SwingUtilities.getWindowAncestor(statusLabel);
                    frame.dispose();  // Cerrar la ventana
                }
            });
        }
    }
    
    public static void main(String[] args) {
        Socket socket = null;
        ServerSocket miServidor = null;
        verificarArchivo verificar;
        int puerto=6000;
        int id = 0;

        try {
            miServidor = new ServerSocket(puerto);
            while(true){
                System.out.println("Recibiendo peticiones en el puerto 6000 esperando peers");
                socket = miServidor.accept();
                id++;
                JOptionPane.showMessageDialog(null, "\nSe conecto el cliente con el id: " + id + " desde la IP: " + socket.getInetAddress(), "Conexión de peers", JOptionPane.INFORMATION_MESSAGE);
                  verificar = new verificarArchivo(socket, id);
                verificar.start();                
            }

        } catch (IOException e) {
            System.out.println(e.getMessage() + " ");
            System.exit(3);
        } finally {
        }
    }
}


