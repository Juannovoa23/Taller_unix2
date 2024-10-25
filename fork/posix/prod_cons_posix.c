void *productor() {
    while(1) {
        pthread_mutex_lock(&condp_mutex);
        while(counter >= 10) {
            pthread_cond_wait(&condVarProd, &condp_mutex);
        }
        pthread_mutex_unlock(&condp_mutex);   


        pthread_mutex_lock(&counter_mutex);
        counter++;
        pthread_cond_signal(&condVarCons);
        printf("Soy productor %d valor contador = %d\n", pthread_self(), counter);
        pthread_mutex_unlock(&condp_mutex);
        pthread_mutex_unlock(&counter_mutex);

        if(counter == 5) {
            sleep(1);
        }
    }
}
void *consumidor() {
    while(1) {
        sleep(1);

        pthread_mutex_lock(&condc_mutex);
        while(counter <= 0) {
            pthread_cond_signal(&condVarProd);
            pthread_cond_wait(&condVarCons, &condc_mutex);   

        }
        pthread_mutex_unlock(&condc_mutex);

        pthread_mutex_lock(&counter_mutex);
        if(counter > 0) {
            printf("Soy consumidor %d valor contador = %d \n", pthread_self(), counter);
            counter--;
            pthread_cond_signal(&condVarProd);
        }
        pthread_mutex_unlock(&counter_mutex);
        pthread_mutex_unlock(&condc_mutex);
    }
}
#include <pthread.h>
#include <stdio.h>

int counter = 0;
int max = 4;

pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condp_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condVarProd = PTHREAD_COND_INITIALIZER;
pthread_cond_t condVarCons = PTHREAD_COND_INITIALIZER;

void   
 *productor() {
    // ... (el código del productor ya lo proporcionaste)
}

void *consumidor() {
    // ... (el código del consumidor ya lo proporcionaste)
}

int main() {
    pthread_t proHilo[max], conHilo[max];

    // Crear hilos productores
    for (int i = 0; i < max; i++) {
        pthread_create(&proHilo[i], NULL, &productor, NULL);
    }

    // Crear hilos consumidores
    for (int i = 0; i < max; i++) {
        pthread_create(&conHilo[i], NULL, &consumidor, NULL);
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < max; i++) {
        pthread_join(proHilo[i], NULL);
        pthread_join(conHilo[i], NULL);
    }

    return 0;
}
