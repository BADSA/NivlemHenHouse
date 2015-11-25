#ifndef NIVLEMHENHOUSE_EXTERNS_H
#define NIVLEMHENHOUSE_EXTERNS_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include "graphics.h"

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
extern time_t START_TIME;
extern int total_food;
extern int total_water;
extern bool simulation_active;

extern pthread_mutex_t	mutex;
extern pthread_cond_t water_cond;
extern pthread_cond_t food_cond;
extern pthread_cond_t nivlem_cond;
extern pthread_cond_t eggs_cond;

extern char TIME[50];

extern int botsinfo_ssock;
extern int botsinfo_csock;
extern int henhouse_ssock;
extern int henhouse_csock;
extern int nivlen_ssock;
extern int nivlen_csock; //client

extern bool water_fill, food_fill, pick_eggs;

#define NIVLEMPORT 10000
#define BOTSPORT 10001
#define HENHOUSEPORT 10002

#endif //NIVLEMHENHOUSE_EXTERNS_H
