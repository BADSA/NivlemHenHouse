/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "../include/server.h"
#include "../include/utils.h"


int sockfd, newsockfd, portno;
socklen_t clilen;
struct sockaddr_in serv_addr, cli_addr;

void init_server(int port_num){
    printf("Starting connection at: %d\n", port_num);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = port_num;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
        error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
}

int accept_connection(){
    newsockfd = accept(sockfd,
                       (struct sockaddr *) &cli_addr,
                       &clilen);

    return newsockfd;
}

char* get_message(int s){
    int sock = s;
    char message[21];
    int n;

    if (sock < 0)
        error("ERROR on accept");
    bzero(message,21);
    //n = read(sock,buffer,20);
    n = recv(sock,message,20,0);
    if (n < 0)
        error("ERROR reading from socket");
    printf("Message: %s\n",message);
    char* bufCopy = (char*) malloc(21 * sizeof(char));
    message[n] = '\0';
    strncpy(bufCopy, message, 20);

    return bufCopy;
}



void *listen_bostinfo_msg(void *args){
    int sock = *((int*) args);
    char message[21];
    int n;
    while(simulation_active){

        if (sock < 0)
            error("ERROR on accept");
        bzero(message,21);
        //n = read(sock,buffer,20);
        n = recv(sock,message,20,0);
        if (n < 0)
            error("ERROR reading from socket");
        printf("Message: %s\n",message);
        char* bufCopy = (char*) malloc(21 * sizeof(char));
        message[n] = '\0';
        strncpy(bufCopy, message, 20);
        pthread_t requestTID;
        pthread_create(&requestTID, NULL, &parse_bootsinfo_msg, (void*) bufCopy);

    }
}

void *listen_henhouse_msg(void *args){
    int sock = *((int*) args);
    char message[21];
    int n;
    while(simulation_active){

        if (sock < 0)
            error("ERROR on accept");
        bzero(message,21);
        //n = read(sock,buffer,20);
        n = recv(sock,message,20,0);
        if (n < 0)
            error("ERROR reading from socket");
        printf("Message: %s\n",message);
        char* bufCopy = (char*) malloc(21 * sizeof(char));
        message[n] = '\0';
        strncpy(bufCopy, message, 20);

        pthread_t requestTID;
        pthread_create(&requestTID, NULL, &parse_henhouse_msg, (void*) bufCopy);

    }
}

void *listen_nivlem_msg(void *args){
    int sock = *((int*) args);
    char message[21];
    int n;
    while(simulation_active){
        if (sock < 0)
            error("ERROR on accept");
        bzero(message,21);
        n = recv(sock,message,20,0);
        if (n < 0)
            error("ERROR reading from socket");

        printf("Message: %s\n",message);
        char* bufCopy = (char*) malloc(20 * sizeof(char));
        message[n] = '\0';
        strncpy(bufCopy, message, 20);
        pthread_t requestTID;
        pthread_create(&requestTID, NULL, &parse_nivlem_msg, (void*) bufCopy);

    }
}