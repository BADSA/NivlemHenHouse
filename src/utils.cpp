#include "../include/utils.h"



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