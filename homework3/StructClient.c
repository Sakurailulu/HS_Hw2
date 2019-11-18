//struct client 
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdbool.h>
#include <string.h>
#include <sys/select.h>
#include <time.h>
#include <sys/types.h>
#include <stdarg.h>
#include <math.h>
#include "DataMessage.h"

#define BUFFER_SIZE 1024
#define MAX_USER_NUM 64
/**
*helper function to initialize the client
*/
void initial_client(struct Client client){
        clients[i].ID = (char*)malloc(BUFFER_SIZE * sizeof(char));
        clients[i].sockfd = -1;
        clients[i].XPos = -1;
        clients[i].YPos = -1;
        clients[i].Range=-1;
}
/*
*function to free client after used
*/
void free_client(struct Client client){
    initial_client(client);
}
/**
 *initialize an fd set with a list of ports, as well as stdin
 * the function will take in two parameter:
 * clients is the array of client structs whose ports we wish to listen to in the fd set set
 * TCP_fd is  the fd set to initialize
*/
fd_set selectOnSockets(const struct client* clients, int TCP_fd)
{
    fd_set set;
    FD_ZERO(&set);
    FD_SET(TCP_fd, &set);
    for (int i=0; i<MAX_CLIENT; ++i) {
        if (clients[i].socket_fd != -1) {
            FD_SET(clients[i].socket_fd, &set);
        }
    }
    select(max_socket(clients,TCP_fd)+1, &set, NULL, NULL, NULL);
    return set;
}