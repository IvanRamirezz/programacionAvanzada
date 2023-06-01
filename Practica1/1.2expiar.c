#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

void expiar(char *fichero, int periodo, char *orden) {
    struct stat estado_inicial;
    struct stat estado_actual;

    // Obtiene el estado inicial del fichero
    stat(fichero, &estado_inicial);

    while (1) {
        // Espera el período de tiempo especificado
        sleep(periodo);

        // Obtiene el estado actual del fichero
        stat(fichero, &estado_actual);

        // Compara el estado actual con el estado inicial
        if (estado_actual.st_mtime != estado_inicial.st_mtime) {
            // El fichero ha sido modificado

            // Carga en memoria y ejecuta la orden
            char comando[256];
            sprintf(comando, "%s \"%s\"", orden, fichero);
            system(comando);

            // Actualiza el estado inicial
            memcpy(&estado_inicial, &estado_actual, sizeof(struct stat));
        }
    }
}

int main(int argc, char *argv[]) {
    // Valores por defecto
    int periodo = 30;
    char *orden = "ls -l";

    // Verifica los argumentos opcionales
    if (argc > 2 && strcmp(argv[2], "-p") == 0) {
        periodo = atoi(argv[3]);
    }

    if (argc > 4) {
        orden = argv[4];
    }

    // Ejecuta la función expiar
    expiar(argv[1], periodo, orden);

    return 0;
}
