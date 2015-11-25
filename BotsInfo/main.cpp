#include <unistd.h>
#include <stdio.h>
#include "../include/globals.h"
#include "../include/server.h"
#include "../include/utils.h"
#include "../include/client.h"
#include "../include/bot.h"
#include <ctime>
#include <iostream>

void *start_simulation(void*){;};

int main() {
    simulation_active = true;
    printf("Conection Wating....\n");
    init_server(BOTSPORT);
    nivlen_ssock = accept_connection();
    printf("Conection Acepted");

    int *sock1 = (int *) malloc(sizeof(int));

    *sock1 = nivlen_ssock;

    pthread_t listen_nivlem;
    pthread_create(&listen_nivlem, NULL, &listen_nivlem_msg, (void *) sock1);

    sleep(1);
    // Send message to nivlem to identify
    nivlen_csock = init_client("localhost", NIVLEMPORT);

    send_message(nivlen_csock,"bots to nivlem");
    pthread_t bot;
    pthread_create(&bot, NULL, &bot_function, NULL);
    srand(time(NULL)); // Seed for random.
    reset_values();
    read_file();
    START_TIME = time(nullptr);
    stats_window();
    printf("Server up");

}
