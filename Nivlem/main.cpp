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
#include "../include/globals.h"
#include "../include/bot.h"
#include "../include/nivlem.h"
#include "../include/workflow.h"
#include "../include/utils.h"
#include "../include/server.h"
#include "../include/client.h"

#include <ctime>


using namespace std;



void *start_simulation(void*){
    simulation_active = true;
    printf("EMPEZANDO SIMULACION \n");

    //Sockets para send
    init_server(NIVLEMPORT);

    henhouse_csock = init_client("localhost", HENHOUSEPORT);

    botsinfo_csock = init_client("localhost", BOTSPORT);

    henhouse_ssock = accept_connection();
    int *sock2 =(int*) malloc(sizeof(int));
    *sock2 = henhouse_ssock;
    pthread_t listen_henhouse;
    pthread_create(&listen_henhouse, NULL,&listen_henhouse_msg,(void*)sock2);

    sleep(1);
    botsinfo_ssock = accept_connection();


    printf("conexiones aceptadas\n");

    int *sock1 =(int*) malloc(sizeof(int));





    *sock1 = botsinfo_ssock;
    pthread_t listen_botsinfo;
    pthread_create(&listen_botsinfo, NULL,&listen_bostinfo_msg,(void*)sock1);



    printf("Ready %d\n", henhouse_csock);
    printf("Ready %d\n", botsinfo_csock);
    send_message(henhouse_csock, "nivlem to henhouse");
    send_message(botsinfo_csock, "nivlem to bot");

    srand(time(NULL)); // Seed for random.

    reset_values();
    read_file();

    START_TIME = std::time(nullptr);
    simulation_active = true;
    pthread_t days_count, nivlem_t, end_simulation;
    pthread_create(&days_count,NULL,&count_days,NULL);
    pthread_create(&nivlem_t, NULL, &nivlem_process, NULL);
    pthread_create(&end_simulation, NULL, &check_simulation_end, NULL);


}

int main(int argc, char **argv){
    printf("Creando ventana de simulacion\n");
    simulation_window();
    printf("Creada\n");
    return 0;
}
