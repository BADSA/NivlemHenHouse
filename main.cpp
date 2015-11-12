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
#include "include/mathfunc.h"
#include "include/globals.h"


using namespace std;

void *start_simulation(void*){
    srand(time(NULL)); // Seed for random.
    START_TIME = clock();
    read_file();
    pthread_t bot, days_count, nivlem;
    pthread_create(&bot, NULL, &bot_function, NULL);
    create_chickens();
    pthread_create(&days_count,NULL,&count_days,NULL);
    pthread_create(&nivlem, NULL, &nivlem_process, NULL);

    while(1){
        clock_t current = clock();
        if(current>= ( START_TIME + SIMULATION_TIME * CLOCKS_PER_SEC)){
            pthread_mutex_lock(&mutex);
            printf("\nFIN DE LA SIMULACIÓN\n");
            printf("=========================================\n");
            printf("Cantidad de huevos: %d \n", eggs_amount + total_eggs);
            printf("Costo total de la simulación: %d \n", cost);
            printf("Duración en días: %d\n",total_days-1);
            printf("Tiempo total de la simulación: %d \n", SIMULATION_TIME);

            pthread_mutex_unlock(&mutex);
            exit(0);
        }
    }
}

int main(int argc, char **argv){

    simulation_window();
    return 0;
}
