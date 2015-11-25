#include "../include/nivlem.h"
#include "../include/client.h"


/*
    Function that receives signal to
    pick the eggs.
*/
void *nivlem_process(void*){
    pthread_t hours_checker;
    pthread_create(&hours_checker, NULL, &wait_hours, NULL);

    while(simulation_active){
        pthread_mutex_lock(&mutex);

        while (eggs_amount < EGGS_MAX && NIVLEM_TIMER > 0)
            pthread_cond_wait(&nivlem_cond, &mutex);

        if(!simulation_active)
            break;
        NIVLEM_TIMER = HOURS_NIVLEM;//* 3600 ;
        printf("\n================ NIVLEM ====================\n");
        printf("Nivlem esta recogiendo los huevos...\n");
        printf("Se recogieron %d huevos.\n",eggs_amount);
        printf("============================================\n");
        //total_eggs += eggs_amount;
        eggs_amount = 0;
        send_message(henhouse_csock, "b-3");

        pthread_mutex_unlock(&mutex);

        pthread_t melvin_rev;
        pthread_create(&melvin_rev, NULL, move_nivlem, (void*) &nivlem);


    }
}


/*
    Waits "NIVLEM_TIMER" to wake up Nivlem
    in order to pick the eggs.
*/
void *wait_hours(void*){
    while(simulation_active){
        while(NIVLEM_TIMER--){
            sleep(1);
        }
        if(!simulation_active)
            break;
        printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
        printf("+ Ya ha pasado las %d horas, se recogerán los huevos + \n", HOURS_NIVLEM);
        printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
        pthread_cond_signal(&nivlem_cond);
    }
}
