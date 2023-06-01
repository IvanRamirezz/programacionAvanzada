#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

void crearHijos(int numHijos) {
    int i;
    pid_t pid;

    // Semilla para la generación de números aleatorios
    srand(time(NULL));

    for (i = 0; i < numHijos; i++) {
        pid = fork();

        if (pid < 0) {
            // Error al crear el hijo
            perror("Error al crear el proceso hijo");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Código del proceso hijo

            // Generar un número aleatorio entre 0 y 30
            int duracion = rand() % 31;

            // Dormir el número de segundos aleatorio
            sleep(duracion);

            // Mostrar información del hijo
            printf("PID: %d, PPID: %d, Duración: %d segundos\n",
                   getpid(), getppid(), duracion);

            // Terminar el proceso hijo
            exit(EXIT_SUCCESS);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <numHijos>\n", argv[0]);
        return 1;
    }

    int numHijos = atoi(argv[1]);

    if (numHijos <= 0) {
        printf("El número de hijos debe ser un entero positivo\n");
        return 1;
    }

    crearHijos(numHijos);

    // Esperar la terminación de cada hijo
    int i;
    pid_t pid;
    int status;

    for (i = 0; i < numHijos; i++) {
        pid = wait(&status);

        if (pid > 0) {
            if (WIFEXITED(status)) {
                printf("Proceso hijo con PID %d terminado con estado %d\n", pid, WEXITSTATUS(status));
            } else {
                printf("Proceso hijo con PID %d terminado de forma anormal\n", pid);
            }
        } else {
            perror("Error al esperar la terminación del proceso hijo");
            return 1;
        }
    }

    return 0;
}
