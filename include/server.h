//
// Created by sd on 11/22/15.
//

#ifndef NIVLEMHENHOUSE_SERVER_H
#define NIVLEMHENHOUSE_SERVER_H

void init_server(int port_num);
void *listen_bostinfo_msg(void *args);
void *listen_henhouse_msg(void *args);
void *listen_nivlem_msg(void *args);
int accept_connection();
char* get_message(int s);

typedef struct s_message_params{
    int socket;
    char *message;
} messages_params;

#endif //NIVLEMHENHOUSE_SERVER_H
