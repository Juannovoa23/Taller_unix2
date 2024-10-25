#include <stdio.h>    // Para printf
#include <string.h>   // Para strcpy, strlen, strcmp
#include <unistd.h>   // Para read, write, close, sleep
#include <fcntl.h>    // Para open y O_RDWR
#include <sys/stat.h> // Para mkfifo
#define FIFO_FILE "/tmp/fifo_twoway"

int main() {
    // Declaramos variables para el archivo, control de finalización, longitud de cadenas, bytes leídos, etc.
    int fd;
    int end_process;
    int stringlen;
    int read_bytes;
    char readbuf[80];
    char end_str[5];

    // Mensaje inicial para el usuario
    printf("FIFO_CLIENT: Envía mensajes infinitamente, para terminar ingresa \"end\"\n");

    // Abrimos el archivo FIFO, si no existe lo creamos
    fd = open(FIFO_FILE, O_CREAT|O_RDWR);

    // Guardamos la palabra clave para terminar el proceso
    strcpy(end_str, "end");

    // Ciclo infinito para seguir leyendo y enviando mensajes
    while (1) {
        // Le pedimos al usuario que ingrese un mensaje
        printf("Ingresa un mensaje: ");
        fgets(readbuf, sizeof(readbuf), stdin);

        // Calculamos la longitud de la cadena y eliminamos el salto de línea ('\n')
        stringlen = strlen(readbuf);
        readbuf[stringlen - 1] = '\0';

        // Comparamos si lo que ingresó el usuario es "end"
        end_process = strcmp(readbuf, end_str);

        if (end_process != 0) {
            // Si no es "end", enviamos el mensaje al servidor
            write(fd, readbuf, strlen(readbuf));
            printf("FIFO_CLIENT: Enviando cadena: \"%s\", longitud: %d\n", readbuf, (int)strlen(readbuf));

            // Esperamos la respuesta del servidor
            read_bytes = read(fd, readbuf, sizeof(readbuf));
            readbuf[read_bytes] = '\0';

            // Mostramos la respuesta del servidor
            printf("FIFO_CLIENT: Recibido cadena: \"%s\", longitud: %d\n", readbuf, (int)strlen(readbuf));
        } else {
            // Si es "end", enviamos el mensaje final y cerramos la conexión
            write(fd, readbuf, strlen(readbuf));
            printf("FIFO_CLIENT: Enviando cadena: \"%s\", longitud: %d\n", readbuf, (int)strlen(readbuf));
            close(fd);
            break;
        }
    }

    return 0;
}
