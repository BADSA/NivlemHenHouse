#include <iostream>
#include <unistd.h>
#include "../include/client.h"

#include "../include/globals.h"
#include "../include/server.h"
#include "../include/utils.h"
#include "../include/workflow.h"

using namespace std;


int main(int argc, char const *argv[])
{
	botsinfo_csock = init_server(10002);
    sleep(1);
    botsinfo_ssock = init_client("localhost", 10001);

    int *sock =(int*) malloc(sizeof(int));


    *sock = botsinfo_csock;
    pthread_t listen_botsinfo;
    pthread_create(&listen_botsinfo, NULL,&receive_messages,(void*)sock);

    char prueba[] = {"p-12"};
    int val = atoi(prueba + 2);
    read_file();
    simulation_active = true;
    create_chickens();
    while(simulation_active);
    printf("%d\n",val);
    printf("Henhouse server UP!!\n");

    //close(socket);
    //receive_messages(socket);


    return 0;

}