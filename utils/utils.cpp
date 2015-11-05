/*

    Instituto Tecnologico de Costa Rica
    Principios de Sistemas Operativos
    Profesor: Eddy Ramírez Jiménez
    Proyecto 2 y 3: Nivlem hen house
    Daniel Solís Méndez
    Melvin Elizondo Pérez

    ==========================DESCRIPCION===============================
    Nivlem es un avicultor preocupado por automatizar todos los procesos
    de su granja de pro- ducción de huevos, buscando tener control de los
    costos de alimento y a la vez saber oportunamente cuando retirar los
    huevos que se han producido.

    El fin de esta simulación consiste en crear diferentes procesos los
    cuales se deben de encargar de simular las gallinas, un bot que se
    encarga de controlar la cantidad de agua y alimento que hay disponible
    y que también lleva el costo total de dinero invertida en estos dos productos
    y finalmente de Nivlem que debe de tener algún tipo de alarma para saber
    cuándo recoger los huevos.

*/

#include "utils.h"
#include <math.h>
#include <stdlib.h>

#define e 2.718281828459046


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
