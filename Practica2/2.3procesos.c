#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2, pid3;
    
    // Proceso 1: Crear una carpeta
    pid1 = fork();
    if (pid1 == 0) {
        mkdir("carpeta", 0777);
        exit(0);
    }
    
    // Esperar a que el proceso 1 termine de crear la carpeta
    wait(NULL);
    
    // Proceso 2: Generar un documento dentro de la carpeta creada
    pid2 = fork();
    if (pid2 == 0) {
        int fd = open("carpeta/documento.txt", O_CREAT | O_WRONLY, 0666);
        close(fd);
        exit(0);
    }
    
    // Esperar a que el proceso 2 termine de generar el documento
    wait(NULL);
    
    // Proceso 3: Escribir en el documento creado por el Proceso 2
    pid3 = fork();
    if (pid3 == 0) {
        int fd = open("carpeta/documento.txt", O_WRONLY | O_APPEND);
        if (fd != -1) {
            write(fd, "Hola Mundo.\n", sizeof("Hola Mundo.\n") - 1);
            close(fd);
        }
        exit(0);
    }
    
    // Esperar a que el proceso 3 termine de escribir en el documento
    wait(NULL);
    
    return 0;
}
