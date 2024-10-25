#include <stdio.h>    // Para printf
#include <string.h>   // Para strcpy, strlen, strcmp
#include <unistd.h>   // Para read, write, close, sleep
#include <fcntl.h>    // Para open y O_RDWR
#include <sys/stat.h>// Definimos el mismo archivo FIFO que el cliente para que ambos se comuniquen
#define FIFO_FILE "/tmp/fifo_twoway"

// Prototipo de la función para invertir cadenas
void reverse_string(char *);

int main() {
    int fd;
    char readbuf[80];
    char end[10];
    int to_end;
    int read_bytes;

    // Creamos el archivo FIFO si no existe, con permisos para lectura/escritura
    mkfifo(FIFO_FILE, S_IFIFO|0640);

    // Guardamos la palabra clave para terminar
    strcpy(end, "end");

    // Abrimos el FIFO para lectura y escritura
    fd = open(FIFO_FILE, O_RDWR);

    // Bucle infinito para procesar mensajes
    while(1) {
        // Leemos lo que envió el cliente
        read_bytes = read(fd, readbuf, sizeof(readbuf));
        readbuf[read_bytes] = '\0'; // Agregamos el terminador de cadena

        printf("FIFOSERVER: Recibido cadena: \"%s\", longitud: %d\n", readbuf, (int)strlen(readbuf));

        // Comparamos si la cadena es "end"
        to_end = strcmp(readbuf, end);
        if (to_end == 0) {
            // Si es "end", cerramos el servidor
            close(fd);
            break;
        }

        // Si no es "end", invertimos la cadena
        reverse_string(readbuf);
        printf("FIFOSERVER: Enviando cadena invertida: \"%s\", longitud: %d\n", readbuf, (int)strlen(readbuf));

        // Enviamos la cadena invertida de vuelta al cliente
        write(fd, readbuf, strlen(readbuf));

        // Pausa para asegurarnos de que el cliente tenga tiempo de leer la respuesta
        sleep(2);
    }

    return 0;
}

// Función para invertir una cadena
void reverse_string(char *str) {
    int last, limit, first;
    char temp;

    // Calculamos el índice del último caracter y la mitad de la cadena
    last = strlen(str) - 1;
    limit = last / 2;
    first = 0;

    // Intercambiamos los caracteres de la cadena desde los extremos hacia el centro
    while (first < last) {
        temp = str[first];
        str[first] = str[last];
        str[last] = temp;
        first++;
        last--;
    }

    return;
}
