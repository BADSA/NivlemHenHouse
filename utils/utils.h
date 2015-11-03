//
// Created by sd on 11/2/15.
//


#ifndef NIVLEMHENHOUSE_UTILS_H
#define NIVLEMHENHOUSE_UTILS_H


/* DEFINES */
#include <vector>

#define e 2.718281828459046
#define vd std::vector<double>

double calc_poisson(double k, double lambda);
long long fact(int n);
vd calcdistr(int lambda);
double getrand10();
int get_wait_time(vd dist);

#endif //NIVLEMHENHOUSE_UTILS_H
