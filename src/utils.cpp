#include "../include/utils.h"
#include "../include/bot.h"
#include "../include/chicken.h"
#include "../include/nivlem.h"
#include "../include/client.h"


void read_file() {

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

void reset_values(){
    total_eggs = 0;
    cost = 0;
    total_days = 1;
    total_food = 0;
    total_water = 0;
    food_amount = 0;
    eggs_amount = 0;
    food_fill = false;
    water_fill = false;
    pick_eggs=false;
    //simulation_active = false;
}

void error(const char *msg){
    perror(msg);
    exit(1);
}

void *parse_henhouse_msg(void *arg) {

    pthread_mutex_lock(&mutex);
    char* message = (char*) arg;
    char* bufCopy = (char*) malloc(21 * sizeof(char));
    strncpy(bufCopy, message, 20);

    send_message(botsinfo_csock,bufCopy);
    printf("Parsing message: %s\n", message);
    int action = message[2]-48;
    int value;
    if(action<3)
        value= atoi(message+4);

    printf("ACTION: %d\n",action);
    switch(action){
        case 1:
            printf("Gallina comiendo %d\n",value);
            food_amount-=value;
            total_food+=value;
            pthread_mutex_unlock(&mutex);
            break;
        case 2:
            printf("Gallina tomando %d\n",value);
            water_amount-=value;
            total_water+=value;
            pthread_mutex_unlock(&mutex);
            break;
        case 3:
            printf("Gallina puso un huevo\n");
            eggs_amount++;
            total_eggs++;
            pthread_mutex_unlock(&mutex);
            break;
        case 6:
            //eggs_amount = 0;
            pthread_cond_signal(&nivlem_cond);
            pthread_mutex_unlock(&mutex);
        default:
            pthread_mutex_unlock(&mutex);
            break;

    }


}

void *parse_bootsinfo_msg(void *arg){
    pthread_mutex_lock(&mutex);
    char* message = (char*) arg;
    char* bufCopy = (char*) malloc(21 * sizeof(char));
    strncpy(bufCopy, message, 20);

    send_message(henhouse_csock,bufCopy);

    printf("Parsing message: %s\n", message);
    int action = message[2]-48;
    int value;
    if(action<3)
        value= atoi(message+4);
    printf("BOT ACTION: %d\n",action);
    switch(action){
        case 1:
            food_amount = value;
            cost+=FOOD_COST;
            pthread_mutex_unlock(&mutex);
            break;
        case 2:
            cost+=WATER_COST;
            water_amount = value;
            pthread_mutex_unlock(&mutex);
            break;
        case 3:
            eggs_amount = 0;
            pthread_cond_signal(&nivlem_cond);
            pthread_mutex_unlock(&mutex);
            break;
        default:
            pthread_mutex_unlock(&mutex);
            break;
    }


}

void *parse_nivlem_msg(void *arg){

    char* message = (char*) arg;

    printf("Parsing message: %s\n", message);
    char code = message[0];
    switch (code){
        case 'c':
            update_general_values(message);
            break;
        case 'b':
            update_chicken_resources(message);
            break;
        case 'x':
            printf("Teniminando la simulacion \n");
            simulation_status(message);
            break;
        default:
            break;
    }
}
