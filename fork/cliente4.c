#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_FILE "/tmp/fifo_twoway"

int main() {
    // Declaramos variables
    int fd;
    char readbuf[80];
    char end_str[] = "end";

    // Abrimos o creamos el FIFO
    fd = open(FIFO_FILE, O_RDWR | O_CREAT);
    if (fd == -1) {
        perror("Error al abrir/crear el FIFO");
        return 1;
    }

    printf("FIFO_CLIENT: Envía mensajes infinitamente, para terminar ingresa \"end\"\n");

    // Bucle principal: se ejecuta hasta que se ingresa "end"
    while (1) {
        printf("Ingresa un mensaje: ");
        fgets(readbuf, sizeof(readbuf), stdin);

        // Eliminamos el salto de línea si existe
        readbuf[strcspn(readbuf, "\n")] = 0;

        // Comparamos si el mensaje es "end"
        if (strcmp(readbuf, end_str) == 0) {
            break; // Salimos del bucle
        }

        // Enviamos el mensaje al servidor
        write(fd, readbuf, strlen(readbuf));
        printf("FIFO_CLIENT: Enviando cadena: \"%s\"\n", readbuf);

        // Recibimos la respuesta del servidor
        int bytes_leidos = read(fd, readbuf, sizeof(readbuf));
        if (bytes_leidos > 0) {
            printf("FIFO_CLIENT: Recibido cadena: \"%s\"\n", readbuf);
        } else if (bytes_leidos == 0) {
            printf("FIFO_CLIENT: El servidor cerró la conexión.\n");
            break;
        } else {
            perror("Error al leer del FIFO");
        }
    }

    // Cerramos el FIFO
    close(fd);

    return 0;
}
