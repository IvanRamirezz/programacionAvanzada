#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2, pid3;
    
    // Proceso 1: Ejecutar la instrucción ls (listSort "lista directorios ") (Que muestre los directorios)
    pid1 = fork();
    if (pid1 == 0) {
        execlp("ls", "ls", NULL);
        exit(0);
    }
    
    // Proceso 2: Ejecutar la instrucción cd Documentos (ingresa a la carpeta Documentos)
    pid2 = fork();
    if (pid2 == 0) {
        chdir("/Users/ivanramirez/Downloads/Documentos");
    if (chdir("/Users/ivanramirez/Downloads/Documentos") == 0) {
        printf("Directorio cambiado correctamente.\n");
    } else {
        printf("Error al cambiar de directorio.\n");
    }
            exit(0);
    }
    
    // Proceso 3: Ejecutar la sentencia pwd (imprime el directorio de trabajo actual)
    pid3 = fork();
    if (pid3 == 0) {
        execlp("pwd", "pwd", NULL);
        exit(0);
    }
    
    // Esperar a que todos los procesos hijos terminen
    wait(NULL);
    wait(NULL);
    wait(NULL);
    
    return 0;
}
