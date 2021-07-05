#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <limits.h>

#include "headers.h"

int countA = 0;
int countB = 0;


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
