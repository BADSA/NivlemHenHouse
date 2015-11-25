#include <iostream>
#include <unistd.h>
#include "../include/server.h"
#include "../include/client.h"

#include "../include/globals.h"

#include "../include/utils.h"
#include "../include/workflow.h"

using namespace std;

void *start_simulation(void*){;};

int main(int argc, char const *argv[])
{
    simulation_active = true;
    init_server(HENHOUSEPORT);
    printf("Conection Waiting\n");

    nivlen_ssock = accept_connection();

    printf("Conection Acepted");
    int *sock1 =(int*) malloc(sizeof(int));

    *sock1 = nivlen_ssock;
    pthread_t nivlem_listener;
    pthread_create(&nivlem_listener, NULL, &listen_nivlem_msg,(void*)sock1);

    // Send message to nivlem to identify
    nivlen_csock = init_client("localhost",NIVLEMPORT);

    send_message(nivlen_csock,"henhouse to nivlem");
    reset_values();
    read_file();
    create_chickens();
    henhouse_window();
    /*
    printf("Henhouse server UP!!\n");*/


    return 0;

}