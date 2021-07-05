#ifndef HEADERS
#define HEADERS

// definitions
#define THREAD_NUM 20   // number of active threads
#define NA 2          // capacity of hallA
#define NB 1          // capacity of hallB
#define PROB_OF_B 0.7                 // probability of entering hallB
#define PROB_OF_ENTERING_AGAIN 0.75    // probability of continuing execution
                                        // by a given thread
#define TIME_MIN 30000     //minimal time of watching exhibition by visitor
#define TIME_MAX 100000     //maximal time of watching exhibition by visitor


// declarations
pthread_t threads[THREAD_NUM];      //declaring threads

pthread_mutex_t mutexHallB;     // door between A and B
pthread_mutex_t mutexHallA;     // door between A and entry
pthread_cond_t condDoorB;
pthread_cond_t condDoorA;


// functions
void* program(void* args);

#endif