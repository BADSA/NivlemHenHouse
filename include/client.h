//
// Created by sd on 11/22/15.
//

typedef struct s_client_params{
    int socket;
    char *message;
} client_params;
#ifndef NIVLEMHENHOUSE_CLIENT_H
#define NIVLEMHENHOUSE_CLIENT_H
int init_client(char const argv[],int port_num);
void send_message(int sock_num, char const *message);
#endif //NIVLEMHENHOUSE_CLIENT_H
