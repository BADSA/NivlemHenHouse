#include "../include/bot.h"
#include "../include/client.h"

/*
    Functions to check for min food and water.
    in order to refill them
*/

pthread_mutex_t	mutex6      = PTHREAD_MUTEX_INITIALIZER;
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
    while(simulation_active) {
        pthread_mutex_lock(&mutex);
        while (food_amount > FOOD_MIN)
            pthread_cond_wait(&food_cond, &mutex);

        printf("\n============== BOT ================\n");
        printf("La cantidad de comida ha llegado al mínimo\n");
        printf("Agregando %d más de comida\n",FOOD_MAX);

        food_amount += FOOD_MAX;
        cost += FOOD_COST;

        char message[256];
        sprintf(message,"b-1-%d",food_amount);
        send_message(henhouse_ssock,message);

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
    while(simulation_active) {
        pthread_mutex_lock(&mutex);
        while (water_amount > WATER_MIN)
            pthread_cond_wait(&water_cond, &mutex);

        printf("\n============== BOT ================\n");
        printf("La cantidad de agua ha llegado al mínimo\n");
        printf("Agregando %d más de agua\n",WATER_MAX);

        water_amount += WATER_MAX;
        cost += WATER_COST;

        char message[256];
        sprintf(message,"b-2-%d",water_amount);
        send_message(henhouse_ssock,message);

        printf("Cantidad actual %d\n",water_amount);
        printf("===================================\n");


        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void update_general_values(char *data){
    pthread_mutex_lock(&mutex);
    int action = data[2]-48;
    int value  = atoi(data+4);
    if(action!=3)
        value = atoi(data+4);
    switch(action){
        case 1:
            food_amount-= value;
            pthread_mutex_unlock(&mutex);
            break;
        case 2:
            water_amount-= value;
            pthread_mutex_unlock(&mutex);
            break;
        case 3:
            eggs_amount++;
            pthread_mutex_unlock(&mutex);
            break;
        case 4:
            pthread_cond_signal(&food_cond);
            pthread_mutex_unlock(&mutex);
            break;
        case 5:
            pthread_cond_signal(&water_cond);
            pthread_mutex_unlock(&mutex);
        default:
            pthread_mutex_unlock(&mutex);
            break;
    }

}