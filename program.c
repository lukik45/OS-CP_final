#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <limits.h>

#include "headers.h"

// PROGRAM 2 --> let visitors from B leave as soon as possible
int countA = 0;
int countB = 0;

int leaveBrequest = 0;



// I can't allow visitors to enter hallA  from outside when there is only one place left in hallA
// because I don't know in advance whether the visitor is willing to enter hallB or not
// this is very important because if I let to a situation that hallB is full
// and hallA is full and all processes from hallA want to enter hallB, then I would 
// have the deadlock
// to prevent this, I decided to use mutexes and conditional variables instead of 
// counting semaphores, even though semaphores seemed to be more intuitive at 
// a first glance.

// mutex locks the counts
// cond var sends signal if some thread left hallB to unlock the mutex
// use property of cond var that it temporaily unlocks the mutex

// mutexes are always nested -> avoid the risk of deadlock

void* program(void* args) {

    int id = *(int*) args;     // id of current thread

    while (1) {
        usleep(TIME_MIN + rand() % (TIME_MAX - TIME_MIN));     // waits some constant ammount of time + [0, 0.5]s
        
        
        pthread_mutex_lock(&mutexHallA);
        while (countA >= NA -1) {   // while there is no more 
                                    // than one free place in A or there is some 
            pthread_cond_wait(&condDoorA, &mutexHallA); // wait for signal that
                                                        // somebody left hallA  
        }
        countA++;      // enter hallA (under closed mutex - safe)
        printf("(%.2d) enters A| A: %.2d\n", id, countA);
        pthread_mutex_unlock(&mutexHallA);

        // ADMIRE PIECES OF ART IN HALL A //
        usleep(TIME_MIN + rand() % (TIME_MAX - TIME_MIN));

        // now visitor decides if he wants to enter hallB
        if(((double) rand() / (double) RAND_MAX) < PROB_OF_B) {
            
            pthread_mutex_lock(&mutexHallB);
            while (countB >= NB) {  // while no fre space in hallB
                pthread_cond_wait(&condDoorB, &mutexHallB); // wait for a signal 
                                                            // that somebody left
                                                            // hallB

            }
            // leave hallA
            pthread_mutex_lock(&mutexHallA); // no need to check any condition
            countA--;
            printf("(%.2d) leaves A| A: %.2d, goes B\n", id, countA);
            pthread_mutex_unlock(&mutexHallA);
            pthread_cond_broadcast(&condDoorB); // tell all visitors waiting on
                                                // condDoorA, that there is some 
                                                // free space in hall A

            // enter hall B
            countB++;
            printf("(%.2d) enters B| B: %.2d\n", id, countB);   
            pthread_mutex_unlock(&mutexHallB);

            // ADMIRE PIECES OF ART IN HALL B //
            usleep(TIME_MIN + rand() % (TIME_MAX - TIME_MIN));

            // now visitor tries to leave hall B and enter hall A once again
            // CAUTION!!!

            // firstly, aquire doorA
            pthread_mutex_lock(&mutexHallA);
            while (countA >= NA) {  // wait till somebody leaves A
                                    // now we don't have to assure 
                                    // there will be one place left 
                                    // if the visitor returns A
                pthread_cond_wait(&condDoorA, &mutexHallA);
            }
            //mutex locked, there is free space in A
            pthread_mutex_lock(&mutexHallB);
            countB--;
            printf("(%.2d) leaves B| B: %.2d\n", id, countB);
            pthread_mutex_unlock(&mutexHallB);
            pthread_cond_broadcast(&condDoorB); // tell all visitors waiting on
                                                // condDoorB, that there is some 
                                                // free space in hall B
            // enter hall A
            countA++;
            printf("(%.2d) enters A| A: %.2d again\n", id, countA);
            pthread_mutex_unlock(&mutexHallA);

            // ADMIRE PIECES OF ART IN HALL A once again //
            usleep(TIME_MIN + rand() % (TIME_MAX - TIME_MIN));
            

        } 
        // now, the visitor wants to leave the museum (hallA)
        //no matter if he entered hallB or not
        pthread_mutex_lock(&mutexHallA);
        countA--;
        printf("(%.2d) leaves A| A: %.2d\n", id, countA);
        pthread_mutex_unlock(&mutexHallA);
        pthread_cond_broadcast(&condDoorA);

        // now the algorithm randomly decides if it runs this thread once again
        // (enter as a new visitor), or break and terminate the thread
        if ((double) rand() / (double) RAND_MAX > PROB_OF_ENTERING_AGAIN)
            break;
    }
}
/*
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

        if (pthread_create(&threads[i], NULL, program1BETA, a) != 0) {
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
}*/