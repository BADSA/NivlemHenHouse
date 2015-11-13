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

#include <iostream>
#include "include/bot.h"
#include "include/nivlem.h"
#include "include/workflow.h"
#include "include/utils.h"
#include "include/globals.h"


using namespace std;

void reset_values(){
    total_eggs = 0;
    cost = 0;
    total_days = 1;
    total_food = 0;
    total_water = 0;
    food_amount = 0;
    eggs_amount = 0;
}

void *start_simulation(void*){
    srand(time(NULL)); // Seed for random.
    START_TIME = clock();
    reset_values();
    read_file();
    simulation_active = true;
    pthread_t bot, days_count, nivlem_t, end_simulation;
    pthread_create(&bot, NULL, &bot_function, NULL);
    create_chickens();
    pthread_create(&days_count,NULL,&count_days,NULL);
    pthread_create(&nivlem_t, NULL, &nivlem_process, NULL);
    pthread_create(&end_simulation, NULL, &check_simulation_end, NULL);

}

int main(int argc, char **argv){

    simulation_window();
    return 0;
}
