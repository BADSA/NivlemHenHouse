#include "../include/utils.h"
#include "../include/bot.h"
#include "../include/chicken.h"



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
}

void error(const char *msg){
    perror(msg);
    exit(1);
}

void *parse_message(void *arg){

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
        case 'n':
            break;
        default:
            break;
    }

}