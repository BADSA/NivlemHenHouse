#include "../include/chicken.h"

/*
    Process of each chicken.
    Every chicken has a thread that runs this function.
*/
void *chicken_process(void * args){
    int chick_num = *((int*) args);
    printf("Creando gallina #%d\n",chick_num);
    pthread_t eat_thread, drink_thread, swot_thread;

    pthread_create(&eat_thread, NULL, &eat, args);
    pthread_create(&drink_thread, NULL, &drink, args);
    pthread_create(&swot_thread, NULL, &swot, args);

    return NULL;
}


/*
    Eat function of the chickens.
    Randomly eats an amount beetween
    FOOD_INTAKE[0] (min) and
    FOOD_INTAKE[1] (max).
*/
void *eat(void *args){
    int chick_num = *((int*) args);
    int time, amount;
    while(1){
        time = get_wait_time(food_dist);
        sleep(time);
        amount = ( rand() % (FOOD_INTAKE[1] - FOOD_INTAKE[0]))+ FOOD_INTAKE[0]; // Random amount of food.
        pthread_mutex_lock(&mutex);
        food_amount -= amount;

        if(food_amount <= FOOD_MIN)
            pthread_cond_signal(&food_cond); // For the bot to refill it.

        printf("La gallina %d comió %d de alimento, la cantidad total de comida es: %d \n",chick_num,amount,food_amount);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}


/*
    Drink function of the chickens.
    Randomly drinks an amount beetween
    WATER_INTAKE[0] (min) and
    WATER_INTAKE[1] (max).
*/
void *drink(void *args){
    int chick_num = *((int*) args);
    int time, amount;
    while(1){
        time = get_wait_time(water_dist);
        sleep(time);
        amount = ( rand() % (WATER_INTAKE[1] - WATER_INTAKE[0]))+ WATER_INTAKE[0]; // Random amount of food.
        pthread_mutex_lock(&mutex);
        water_amount -= amount;

        if(water_amount <= WATER_MIN)
            pthread_cond_signal(&water_cond); // For the bot to refill it.
        printf("La gallina %d tomó %d mililitros agua, la cantidad de agua total es: %d \n", chick_num, amount, water_amount);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}


/*
    Swot function of the chickens.
    Randomly puts an egg.
*/
void *swot(void *args){
    int chick_num = *((int*) args);
    int time;
    while(1){
        time = get_wait_time(egg_dist);
        sleep(time);
        pthread_mutex_lock(&mutex);
        eggs_amount++;
        printf("La gallina %d puso un huevo, cantidad de huevos: %d\n",chick_num,eggs_amount);

        if(eggs_amount >= EGGS_MAX){
            printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
            printf("+ La canasta se ha llenado, Nivlem recogerá los huevos +\n");
            printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
            pthread_cond_signal(&nivlem_cond);
        }

        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}