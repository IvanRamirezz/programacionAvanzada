#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;  // Declaración de environ

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: ejecutar <opción>\n");
        return 1;
    }

    if (argv[1][0] == '-') {
        switch (argv[1][1]) {
            case 'l':
                execl("/bin/ls", "ls", "-al", "/usr/bin", NULL);
                break;
            case 'v':
                execv("/bin/ls", (char *[]){"ls", "-al", "/usr/bin", NULL});
                break;
            case 'e':
                execle("/bin/ls", "ls", "-al", "/usr/bin", NULL, environ);
                break;
            default:
                fprintf(stderr, "Opción no válida: %s\n", argv[1]);
                return 1;
        }
    } else {
        fprintf(stderr, "Opción no válida: %s\n", argv[1]);
        return 1;
    }

    return 0;
}
//En resumen, el programa es una forma de invocar el comando ls -al /usr/bin 
//utilizando diferentes variantes de la función exec según la opción proporcionada. 
//Esto te permite explorar los archivos y directorios en /usr/bin y ver información detallada sobre ellos.