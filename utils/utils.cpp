//
// Created by sd on 11/2/15.
//

#include "utils.h"
#include <math.h>
#include <stdlib.h>




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
    vd dist_acum;

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

int get_wait_time(vd dist){
    double p = getrand10();
    for (int i = 0; i < dist.size(); i++)
        if (p < dist[i]) return i+1;
    return dist.size();
}
