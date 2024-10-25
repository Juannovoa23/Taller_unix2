#include <stdio.h>    // Biblioteca para funciones de entrada y salida estándar (printf)
#include <stdlib.h>   // Biblioteca para funciones generales de propósito (exit)
#include <pthread.h>  // Biblioteca para trabajar con hilos (pthread)
#include <unistd.h>   // Biblioteca para funciones POSIX (sleep)

// Variables compartidas
int counter = 0;      // Contador global que indica el número de elementos producidos
int max = 4;          // Número máximo de hilos productores y consumidores

// Declaración de mutex para sincronización
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex para proteger el acceso a 'counter'
pthread_mutex_t condp_mutex = PTHREAD_MUTEX_INITIALIZER;   // Mutex para controlar la condición del productor
pthread_mutex_t condc_mutex = PTHREAD_MUTEX_INITIALIZER;   // Mutex para controlar la condición del consumidor

// Variables de condición para señalización entre hilos
pthread_cond_t condVarProd = PTHREAD_COND_INITIALIZER; // Variable de condición para los productores
pthread_cond_t condVarCons = PTHREAD_COND_INITIALIZER; // Variable de condición para los consumidores

// Función para el hilo productor
void *productor() {
    while(1) { // Bucle infinito para que el productor siempre esté produciendo
        pthread_mutex_lock(&condp_mutex); // Bloquea el mutex para controlar la condición del productor
        while(counter >= 10) { // Si el contador es mayor o igual a 10, espera
            pthread_cond_wait(&condVarProd, &condp_mutex); // El productor espera hasta que el consumidor lo notifique
        }
        pthread_mutex_unlock(&condp_mutex); // Desbloquea el mutex de condición del productor

        pthread_mutex_lock(&counter_mutex); // Bloquea el mutex para modificar el contador
        counter++; // Incrementa el contador
        pthread_cond_signal(&condVarCons); // Notifica al consumidor que hay un producto disponible
        printf("Soy productor %d valor contador = %d\n", (int)pthread_self(), counter); // Imprime el ID del hilo y el valor del contador
        pthread_mutex_unlock(&counter_mutex); // Desbloquea el mutex del contador
        pthread_mutex_unlock(&condp_mutex); // Desbloquea el mutex de condición del productor

        if(counter == 5) // Si el contador es 5
            sleep(1); // El hilo productor duerme por 1 segundo
    }
}

// Función para el hilo consumidor
void *consumidor() {
    while(1) { // Bucle infinito para que el consumidor siempre esté consumiendo
        sleep(1); // El hilo consumidor duerme por 1 segundo
        pthread_mutex_lock(&condc_mutex); // Bloquea el mutex para controlar la condición del consumidor
        while(counter <= 0) { // Si el contador es menor o igual a 0, espera
            pthread_cond_signal(&condVarProd); // Notifica al productor que puede producir
            pthread_cond_wait(&condVarCons, &condc_mutex); // El consumidor espera hasta que el productor lo notifique
        }
        pthread_mutex_unlock(&condc_mutex); // Desbloquea el mutex de condición del consumidor

        pthread_mutex_lock(&counter_mutex); // Bloquea el mutex para modificar el contador
        if(counter > 0) { // Si hay productos disponibles
            printf("Soy consumidor %d valor contador = %d\n", (int)pthread_self(), counter); // Imprime el ID del hilo y el valor del contador
            counter--; // Decrementa el contador
            pthread_cond_signal(&condVarProd); // Notifica al productor que puede producir
        }
        pthread_mutex_unlock(&counter_mutex); // Desbloquea el mutex del contador
        pthread_mutex_unlock(&condc_mutex); // Desbloquea el mutex de condición del consumidor
    }
}

// Función principal
int main() {
    pthread_t proHilo[max], conHilo[max]; // Arreglos de hilos para productores y consumidores
    int i; // Variable de control para los bucles

    for(i = 0; i < 5; i++) { // Crear 5 hilos productores y 5 consumidores
        pthread_create(&proHilo[i], NULL, &productor, NULL); // Crea un hilo productor
        pthread_create(&conHilo[i], NULL, &consumidor, NULL); // Crea un hilo consumidor
    }

    for(i = 0; i < 5; i++) { // Esperar a que terminen todos los hilos
        pthread_join(proHilo[i], NULL); // Unir (esperar) a los hilos productores
        pthread_join(conHilo[i], NULL); // Unir (esperar) a los hilos consumidores
    }

    return 0; // Finaliza el programa
}
