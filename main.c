#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <limits.h>

#include "headers.h"




int main() {

    // initialisation
    pthread_mutex_init(&mutexHallA, NULL);
    pthread_mutex_init(&mutexHallB, NULL);
    pthread_cond_init(&condDoorA, NULL);
    pthread_cond_init(&condDoorB, NULL);

    // creating threads
    for (int i = 0; i < THREAD_NUM; i++) {

        // I want to pass ID of a thread to the thread
        int* a = malloc(sizeof(int));
        *a = i;

        if (pthread_create(&threads[i], NULL, program, a) != 0) {
            perror("Failed creating threads");
            return 2;
        }
    }

    
    // joining threads
    for (int i = 0; i < THREAD_NUM; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Failed joining threads");
            return 3;
        }
    }
    pthread_mutex_destroy(&mutexHallA);
    pthread_mutex_destroy(&mutexHallB);
    pthread_cond_destroy(&condDoorA);
    pthread_cond_destroy(&condDoorB);


    return 0;
}