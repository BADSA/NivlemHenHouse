#ifndef NIVLEMHENHOUSE_GLOBALS_H
#define NIVLEMHENHOUSE_GLOBALS_H

#include <time.h>
#include <vector>
#include <pthread.h>
#include <vector>
#define vd std::vector<double>


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
int total_food =0 ;
int total_water = 0;
int NIVLEM_TIMER;
bool simulation_active;
clock_t START_TIME;

pthread_mutex_t	mutex      = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t water_cond  = PTHREAD_COND_INITIALIZER;
pthread_cond_t food_cond   = PTHREAD_COND_INITIALIZER;
pthread_cond_t nivlem_cond = PTHREAD_COND_INITIALIZER;

#endif //NIVLEMHENHOUSE_GLOBALS_H
