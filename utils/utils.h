//
// Created by sd on 11/2/15.
//
/* DEFINES */
#include <vector>
#define vd std::vector<double>

#ifndef NIVLEMHENHOUSE_UTILS_H
#define NIVLEMHENHOUSE_UTILS_H




double calc_poisson(double k, double lambda);
long long fact(int n);
vd calcdistr(int lambda);
double getrand10();
int get_wait_time(vd dist);

#endif //NIVLEMHENHOUSE_UTILS_H
