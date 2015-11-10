/*

    Instituto Tecnologico de Costa Rica
    Principios de Sistemas Operativos
    Profesor: Eddy Ramírez Jiménez
    Proyecto 2 y 3: Nivlem hen house
    Daniel Solís Méndez
    Melvin Elizondo Pérez

    ==========================DESCRIPCION===============================
    Nivlem es un avicultor preocupado por automatizar todos los procesos
    de su granja de pro- ducción de huevos, buscando tener control de los
    costos de alimento y a la vez saber oportunamente cuando retirar los
    huevos que se han producido.

    El fin de esta simulación consiste en crear diferentes procesos los
    cuales se deben de encargar de simular las gallinas, un bot que se
    encarga de controlar la cantidad de agua y alimento que hay disponible
    y que también lleva el costo total de dinero invertida en estos dos productos
    y finalmente de Nivlem que debe de tener algún tipo de alarma para saber
    cuándo recoger los huevos.

*/

#include <iostream>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <map>
#include <allegro5/allegro.h>
#include "utils/utils.h"
#include "utils/graphics.h"

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
int total_eggs = 0;
int HOURS_NIVLEM = 10;
int total_days = 1;
int NIVLEM_TIMER;
clock_t START_TIME;


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
    Reads the values of the
    configuration to run.
*/
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
    cost = FOOD_COST + WATER_COST;

    water_dist = calcdistr(water_lambda);
    food_dist = calcdistr(food_lambda);
    egg_dist = calcdistr(eggs_lambda);

    NIVLEM_TIMER = HOURS_NIVLEM;// * 3600;

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
    Waits "NIVLEM_TIMER" to wake up Nivlem
    in order to pick the eggs.
*/
void *wait_hours(void*){
    while(1){
        while(NIVLEM_TIMER--){
            sleep(1);
        }
        printf("\n++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
        printf("+ Ya ha pasado las %d horas, se recogerán los huevos + \n",HOURS_NIVLEM);
        printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
        pthread_cond_signal(&nivlem_cond);
    }
}

/*
    Function that receives signal to
    pick the eggs.
*/
void *nivlem_process(void*){
    pthread_t hours_checker;
    pthread_create(&hours_checker, NULL, &wait_hours, NULL);

    while(1){
        pthread_mutex_lock(&mutex);

        while (eggs_amount < EGGS_MAX && NIVLEM_TIMER > 0)
            pthread_cond_wait(&nivlem_cond, &mutex);

        NIVLEM_TIMER = HOURS_NIVLEM;//* 3600 ;
        printf("\n================ NIVLEM ====================\n");
        printf("Nivlem esta recogiendo los huevos...\n");
        printf("Se recogieron %d huevos.\n",eggs_amount);
        printf("============================================\n");
        total_eggs += eggs_amount;
        eggs_amount = 0;

        pthread_mutex_unlock(&mutex);

    }
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
            printf("Tiempo total de la simulación: %d \n", SIMULATION_TIME);

            pthread_mutex_unlock(&mutex);
            exit(0);
        }
    }
    return NULL;
}



/*
    STARTS HERE
*/
int main(int argc, char **argv){
    /*
    srand(time(NULL)); // Seed for random.
    START_TIME = clock();
    read_input();

    pthread_t bot;
    pthread_create(&bot, NULL, &bot_function, NULL);

    create_chickens();

    pthread_t days_count;
    pthread_create(&days_count,NULL,&count_days,NULL);

    pthread_t nivlem;
    pthread_create(&nivlem, NULL, &nivlem_process, NULL);

    //pthread_t end_checker;
    //pthread_create(&end_checker, NULL, &check_simulation_end, NULL);

    printf("Running\n");
    while(1){
        clock_t current = clock();
        if(current>= ( START_TIME + SIMULATION_TIME * CLOCKS_PER_SEC)){
            pthread_mutex_lock(&mutex);
            printf("\nFIN DE LA SIMULACIÓN\n");
            printf("=========================================\n");
            printf("Cantidad de huevos: %d \n", eggs_amount + total_eggs);
            printf("Costo total de la simulación: %d \n", cost);
            printf("Duración en días: %d\n",total_days);
            printf("Tiempo total de la simulación: %d \n", SIMULATION_TIME);

            pthread_mutex_unlock(&mutex);
            exit(0);
        }
    }
    */
    simulation_window();

    return 0;
}
