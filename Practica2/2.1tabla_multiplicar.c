#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int i;

    for (i = 1; i <= 10; i++) {
        pid = fork();

        if (pid == 0) {
            printf("Tabla de multiplicar del %d:\n", i);
            for (int j = 1; j <= 10; j++) {
                printf("%d x %d = %d\n", i, j, i * j);
            }
            return 0;
        }
    }

    // Esperar a que todos los procesos hijos terminen
    for (i = 1; i <= 10; i++) {
        wait(NULL);
    }

    return 0;
}
