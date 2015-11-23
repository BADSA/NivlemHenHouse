//
// Created by sd on 11/22/15.
//

#ifndef NIVLEMHENHOUSE_SERVER_H
#define NIVLEMHENHOUSE_SERVER_H

int init_server(int port_num);
void *receive_messages(void *args);

typedef struct s_message_params{
    int socket;
    char *message;
} messages_params;

#endif //NIVLEMHENHOUSE_SERVER_H
