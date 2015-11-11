#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../include/chicken.h"
#include "../include/externs.h"
#include "../include/workflow.h"

/*
    Creates a thread for all the N chickens.
*/
void create_chickens(){
    pthread_t *chickens = (pthread_t*) calloc(CHICKENS_AMOUNT, sizeof(pthread_t));
    int *id;
    for (int cn = 0; cn < CHICKENS_AMOUNT; cn++){
        id = (int*)malloc(sizeof(int));
        *id = cn+1;
        pthread_create(&chickens[cn], NULL, &chicken_process, (void *) id);
    }
}


/*
    Thread function to count
    the days that has passed since the
    execution start.
*/
void *count_days(void*){
    while(1){
        printf("\n##############################\n");
        printf("Dia numero: %d ...\n",total_days++);
        printf("##############################\n");
        sleep(DAY_DURATION);
    }
    return NULL;
}


/*
    Checks whether or not
    the simulation end.
    Prints final Stats.
*/
void *check_simulation_end(void*){
    printf("Running\n");
    while(1){
        clock_t current = clock();
        if(current>= ( START_TIME + SIMULATION_TIME * CLOCKS_PER_SEC)){
            pthread_mutex_lock(&mutex);
            printf("\nFIN DE LA SIMULACIÓN\n");
            printf("=========================================\n");
            printf("Cantidad de huevos: %d \n", eggs_amount + total_eggs);
            printf("Costo total de la simulación: %d \n", cost);
            printf("Duración en días: %d\n",total_days-1);
            printf("Tiempo total de la simulación: %d \n", SIMULATION_TIME);

            pthread_mutex_unlock(&mutex);
            exit(0);
        }
    }
    return NULL;
}