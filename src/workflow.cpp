#include "../include/workflow.h"
#include "../include/client.h"

pthread_t *chickens;
/*
    Creates a thread for all the N chickens.
*/
void create_chickens(){
    printf("%d\n ========================\n", CHICKENS_AMOUNT);
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
    time_t current;
    while(simulation_active){
        current  = time(nullptr);
        double result = SIMULATION_TIME - (difftime(current,START_TIME));
        if(result <= 0){
            pthread_mutex_lock(&mutex);
            printf("\nFIN DE LA SIMULACIÓN\n");
            printf("=========================================\n");
            printf("Cantidad de huevos: %d \n", total_eggs);
            printf("Costo total de la simulación: %d \n", cost);
            printf("Duración en días: %d\n",total_days-1);
            printf("Tiempo total de la simulación: %d \n", SIMULATION_TIME);
            printf("TOtal eggs %d\n",total_eggs);
            printf("egg_amount %d\n",eggs_amount);
            simulation_active = false;
            pthread_mutex_unlock(&mutex);
            send_message(henhouse_csock,"x-0");
            simulation_active = false;
            break;
        }

    }
    return NULL;
}

void simulation_status(char *data){
    printf("SIMULATION STATUS\n");
    pthread_mutex_lock(&mutex);
    simulation_active = false;
    pthread_mutex_unlock(&mutex);


}