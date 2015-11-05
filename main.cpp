#include <iostream>
#include <vector>
#include <stdio.h>
#include "utils/utils.h"
#include <unistd.h>
#include <pthread.h>
#include <map>
#include <allegro5/allegro.h>

using namespace std;

/* VARS */
pthread_mutex_t	mutex      = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t water_cond  = PTHREAD_COND_INITIALIZER;
pthread_cond_t food_cond   = PTHREAD_COND_INITIALIZER;
pthread_cond_t nivlem_cond = PTHREAD_COND_INITIALIZER;

vd egg_dist, water_dist, food_dist;

int food_amount = 10, FOOD_MAX = 10;
int water_amount = 10, WATER_MAX = 10;
int WATER_COST = 40, FOOD_COST = 100;
int FOOD_MIN = 2, WATER_MIN = 1;
int CHICKENS_AMOUNT = 10;
int cost = 0, eggs_amount = 0;
int DAY_DURATION = 288;
int FOOD_INTAKE[2], WATER_INTAKE[2];
int EGGS_MAX, SIMULATION_TIME;
int total_eggs;
int HOURS_NIVLEM = 6;
int NIVLEM_TIMER;

clock_t START_TIME;
/*
    Keep checking the food to refill it
    when it reaches the minimun.
*/
void* check_food(void*){
    while(1) {
        pthread_mutex_lock(&mutex);
        while (food_amount > FOOD_MIN)
            pthread_cond_wait(&food_cond, &mutex);

        food_amount += FOOD_MAX;
        cost += FOOD_COST;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}


/*
    Keep checking the water to refill it
    when it reaches the minimun.
*/
void* check_water(void*){
    while(1) {
        pthread_mutex_lock(&mutex);
        while (water_amount > WATER_MIN)
            pthread_cond_wait(&water_cond, &mutex);
        water_amount += WATER_MAX;
        cost += WATER_COST;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}


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

        printf("La gallina %d comio' %d de comida, la cantidad total de comida es: %d \n",chick_num,amount,food_amount);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

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
        printf("La gallina %d tomo' %d mililitros agua, la cantidad de agua total es: %d \n", chick_num, amount, water_amount);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *swot(void *args){
    int chick_num = *((int*) args);
    int time;
    while(1){
        time = get_wait_time(egg_dist);
        sleep(time);
        pthread_mutex_lock(&mutex);
        eggs_amount++;
        if(eggs_amount >= EGGS_MAX){
            printf("La canasta se ha llenado, Nivlem recogera' los huevos\n");
            pthread_cond_signal(&nivlem_cond);
        }


        printf("La gallina %d puso un huevo, cantidad de huevos: %d\n",chick_num,eggs_amount);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *chicken_process(void * args){
    int chick_num = *((int*) args);
    printf("Creating chicken #%d\n",chick_num);
    pthread_t eat_thread, drink_thread, swot_thread;

    pthread_create(&eat_thread, NULL, &eat, args);
    pthread_create(&drink_thread, NULL, &drink, args);
    pthread_create(&swot_thread, NULL, &swot, args);

    return NULL;
}


void create_chickens(){
    pthread_t *chickens = (pthread_t*) calloc(CHICKENS_AMOUNT, sizeof(pthread_t));
    int *id;
    for (int cn = 0; cn < CHICKENS_AMOUNT; cn++){
        id = (int*)malloc(sizeof(int));
        *id = cn+1;
        pthread_create(&chickens[cn], NULL, &chicken_process, (void *) id);
    }
}

void read_input(){
    int water_lambda, food_lambda;
    int eggs_lambda;

    freopen("input","r",stdin);

    int input[15], cont = 0;
    while(scanf("%d",&input[cont++]) != EOF);

    //Setting necessary variables
    water_lambda = input[0];
    food_lambda = input[1];
    eggs_lambda = input[2];

    WATER_INTAKE[0] = input[3];
    WATER_INTAKE[1] = input[4];
    FOOD_INTAKE[0] = input[5];
    FOOD_INTAKE[1] = input[6];

    WATER_MIN = input[7];
    WATER_MAX = input[8];

    FOOD_MIN = input[9];
    FOOD_MAX = input[10];

    CHICKENS_AMOUNT = input[11];

    EGGS_MAX = input[12];

    WATER_COST = input[13];

    FOOD_COST = input[14];

    SIMULATION_TIME = input[15];


    food_amount = FOOD_MAX;
    water_amount = WATER_MAX;

    water_dist = calcdistr(water_lambda);
    food_dist = calcdistr(food_lambda);
    egg_dist = calcdistr(eggs_lambda);

    NIVLEM_TIMER = HOURS_NIVLEM * 3600;

}


/*  3600
void nivlemProc() {
    clock_t last = clock();

    while (1) {
        clock_t current = clock();
        pthread_mutex_lock(&mutex);
        if ((current >= (last + NIVLEM_WAIT * CLOCKS_PER_SEC))) {
            printf("Han pasado 6 horas, se recogeran los huevos\n");
            total_eggs += eggs_amount;
            eggs_amount = 0;

            last = current;
        }
        else if (egg_amount >= EGGS_MAX) {
            printf("Se llego al maximo de huevos en la canasta, se recogeran\n");
            egg_amount = 0;
            last = current;
        }
        pthread_mutex_unlock(&mutex);
    }
}
 */

void *count_days(){
    int day=1;
    while(1){
        printf("Dia numero: %d ...\n",day++);
        sleep(DAY_DURATION);
    }
    return NULL;
}

void *wait_hours(){
    while(1){
        while(NIVLEM_TIMER--){
            sleep(1);
        }
        printf("Ya ha pasado las %d horas, se recogeran los huevos\n",HOURS_NIVLEM);
        pthread_cond_signal(&nivlem_cond);
    }
}


void *nivlem_process(){
    while(1){
        pthread_cond_wait(&nivlem_cond, &mutex);
        pthread_mutex_lock(&mutex);
        NIVLEM_TIMER = 3600 * HOURS_NIVLEM;
        total_eggs += eggs_amount;
        eggs_amount = 0;
        pthread_mutex_unlock(&mutex);
    }
}

void *end_simulation(){
    while(1){
        clock_t current = clock();

        if(current>= (START_TIME+SIMULATION_TIME * CLOCKS_PER_SEC)){
            pthread_mutex_lock(&mutex);

            printf("Cantidad de huevos: %d \n", eggs_amount);
            printf("Costo total de la simulacio'n: %d \n", cost);
            printf("Tiempo total de la simulacion: %d \n", SIMULATION_TIME);

            pthread_mutex_unlock(&mutex);
            exit(0);
        }
    }
    return NULL;
}
int main(){

    START_TIME = clock();
    srand(time(NULL)); // Seed for random.
    read_input();
    pthread_t days_count;
    pthread_create(&days_count,NULL,&count_days,NULL);

    pthread_t bot;
    pthread_create(&bot, NULL, &bot_function, NULL);

    create_chickens();
    return 0;
}