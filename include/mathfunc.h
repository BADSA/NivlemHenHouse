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

/* DEFINES */
#include "externs.h"

#ifndef NIVLEMHENHOUSE_MATHFUNC_H
#define NIVLEMHENHOUSE_MATHFUNC_H

double calc_poisson(double k, double lambda);
long long fact(int n);
vd calcdistr(int lambda);
double getrand10();
int get_wait_time(vd dist);

#endif
