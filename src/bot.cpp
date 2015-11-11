#include <stdio.h>
#include <pthread.h>
#include "../include/externs.h"
#include "../include/bot.h"

/*
    Functions to check for min food and water.
    in order to refill them
*/
void *bot_function(void*){
    pthread_t food_bot, water_bot;
    pthread_create(&food_bot, NULL, &check_food, NULL);
    pthread_create(&water_bot, NULL, &check_water, NULL);
    return NULL;
}


/*
    Keep checking the food to refill it
    when it reaches the minimun.
*/
void *check_food(void*){
    while(1) {
        pthread_mutex_lock(&mutex);
        while (food_amount > FOOD_MIN)
            pthread_cond_wait(&food_cond, &mutex);

        printf("\n============== BOT ================\n");
        printf("La cantidad de comida ha llegado al mínimo\n");
        printf("Agregando %d más de comida\n",FOOD_MAX);

        food_amount += FOOD_MAX;
        cost += FOOD_COST;

        printf("Cantidad actual %d\n",food_amount);
        printf("===================================\n");

        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}


/*
    Keep checking the water to refill it
    when it reaches the minimun.
*/
void *check_water(void*){
    while(1) {
        pthread_mutex_lock(&mutex);
        while (water_amount > WATER_MIN)
            pthread_cond_wait(&water_cond, &mutex);

        printf("\n============== BOT ================\n");
        printf("La cantidad de agua ha llegado al mínimo\n");
        printf("Agregando %d más de agua\n",WATER_MAX);

        water_amount += WATER_MAX;
        cost += WATER_COST;

        printf("Cantidad actual %d\n",water_amount);
        printf("===================================\n");


        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}