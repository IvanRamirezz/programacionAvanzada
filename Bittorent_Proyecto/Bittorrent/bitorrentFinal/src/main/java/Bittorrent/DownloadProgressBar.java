/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package Bittorrent;
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class DownloadProgressBar extends JFrame {
    private JProgressBar progressBar;
    private JLabel statusLabel;

    public DownloadProgressBar(int fileSize){
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

        setVisible(true);
    }

    public static void main(String[] args) {
        int fileSize = 20000;  // Tamaño del archivo en MB
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                new DownloadProgressBar(fileSize);
            }
        });
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
                    Thread.sleep(500);  // Simular tiempo de descarga
                } catch (InterruptedException ex) {
                    ex.printStackTrace();
                }
                progressBar.setValue(progress);
                progress += 10;
            }

            try {
                Thread.sleep(5000);  // Esperar 5 segundos
            } catch (InterruptedException ex) {
                ex.printStackTrace();
            }

            statusLabel.setText("Descarga completada");

            try {
                Thread.sleep(3000);  // Esperar 5 segundos antes de cerrar la ventana
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
}
