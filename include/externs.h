#ifndef NIVLEMHENHOUSE_EXTERNS_H
#define NIVLEMHENHOUSE_EXTERNS_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define vd std::vector<double>


extern vd egg_dist, water_dist, food_dist;

extern int food_amount, FOOD_MAX;
extern int water_amount, WATER_MAX;
extern int WATER_COST, FOOD_COST;
extern int FOOD_MIN, WATER_MIN;
extern int CHICKENS_AMOUNT;
extern int cost, eggs_amount;
extern int DAY_DURATION;
extern int FOOD_INTAKE[2], WATER_INTAKE[2];
extern int EGGS_MAX, SIMULATION_TIME;
extern int total_eggs;
extern int HOURS_NIVLEM;
extern int total_days;
extern int NIVLEM_TIMER;
extern clock_t START_TIME;

extern pthread_mutex_t	mutex;
extern pthread_cond_t water_cond;
extern pthread_cond_t food_cond;
extern pthread_cond_t nivlem_cond;

#endif //NIVLEMHENHOUSE_EXTERNS_H
