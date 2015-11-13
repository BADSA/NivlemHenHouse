#include "../include/workflow.h"
pthread_t *chickens;
/*
    Creates a thread for all the N chickens.
*/
void create_chickens(){
    chickens = (pthread_t*) calloc(CHICKENS_AMOUNT, sizeof(pthread_t));
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
    while(simulation_active){
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
    while(simulation_active){
        clock_t current = clock();
        if(current >= ( START_TIME + SIMULATION_TIME * CLOCKS_PER_SEC)){
            pthread_mutex_lock(&mutex);
            printf("\nFIN DE LA SIMULACIÓN\n");
            printf("=========================================\n");
            printf("Cantidad de huevos: %d \n", eggs_amount + total_eggs);
            printf("Costo total de la simulación: %d \n", cost);
            printf("Duración en días: %d\n",total_days-1);
            printf("Tiempo total de la simulación: %d \n", SIMULATION_TIME);
            simulation_active = false;
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}
