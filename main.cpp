#include <iostream>
#include <time.h>
#include <vector>
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"

/* DEFINES */
#define e 2.718281828459046
#define vd vector<double>

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
int cost;


long long fact(int n){
    long long res = 1;
    for(int i = 2; i <= n; i++) res *= i;
    return res;
}

double calc_poisson(double k, double lambda){
    return ( (pow(e, 0-lambda) * pow(lambda, k)) / fact(k) );
}

/*
    Calcutes poisson's acumulated
    using the lambda received.
*/
vd calcdistr(int lambda){
    int i = 1;
    double acum = 0, prob_i;
    vector<double> dist_acum;

    while(acum < 0.99){
        prob_i = calc_poisson(i++,lambda);
        if (acum + prob_i > 1.0 ||
            acum + prob_i < acum + 0.000001) break; // Stop here
        acum += prob_i;
        dist_acum.push_back(acum);
    }
    return dist_acum;
}

/*
    Generates a random number between 1 and 0.
*/
double getrand10(){
    return (double)rand() / (double)RAND_MAX ;
}

int wait_time(vd dist){
    double p = getrand10();
    for (int i = 0; i < dist.size(); i++)
        if (p < dist[i]) return i+1;
    return dist.size();
}


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


int main()
{


    srand(time(NULL)); // Seed for random.

    egg_dist = calcdistr(7);
    water_dist = calcdistr(5);
    food_dist = calcdistr(6);


    pthread_t bot;

    pthread_create(&bot, NULL, &bot_function, NULL);


    return 0;
}
