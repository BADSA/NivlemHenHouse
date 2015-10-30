#include <iostream>
#include <time.h>
#include <vector>
#include "math.h"
#include "stdio.h"
#include "stdlib.h"

/* DEFINES */
#define e 2.718281828459046
#define vd vector<double>

using namespace std;

/* VARS */
vd egg_dist, water_dist, food_dist;



long long fact(int n){
    long long res = 1;
    for(int i = 2; i <= n; i++) res *= i;
    return res;
}

double calc_poisson(double k, double lambda){
    return (pow(e, 0-lambda) * pow(lambda, k)) / fact(k);
}

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

double getrand10(){
    return (double)rand() / (double)RAND_MAX ;
}

int wait_time(vd dist){
    double p = getrand10();
    for (int i = 0; i < dist.size(); i++){
        if (p < dist[i]) return i+1;
    }
    return dist.size();
}



int main()
{
    egg_dist = calcdistr(7);
    water_dist = calcdistr(5);
    food_dist = calcdistr(6);
    srand(time(NULL)); // Seed for random.


    return 0;
}
