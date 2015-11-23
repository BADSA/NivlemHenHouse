#include <unistd.h>
#include <stdio.h>
#include "../include/globals.h"
#include "../include/server.h"
#include "../include/utils.h"
#include "../include/client.h"
#include "../include/bot.h"

int main(){
    henhouse_ssock = init_client("localhost",10002);
    botsinfo_ssock = init_server(10001);
    int *sock =(int*) malloc(sizeof(int));
    reset_values();
    read_file();
    *sock = botsinfo_ssock;
    pthread_t listen_henhouse;
    pthread_create(&listen_henhouse, NULL,&receive_messages,(void*)sock);

    pthread_t bot;
    pthread_create(&bot, NULL, &bot_function, NULL);
    simulation_active = true;
    while(simulation_active);
    printf("Server up");
    //close(socked);
    //receive_messages(socked);
}