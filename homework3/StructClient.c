//struct client 

#include "StructClient.h"


/**
*helper function to initialize the client
*/
void initial_client(struct Client client){
        client.ID = (char*)malloc(BUFFER_SIZE * sizeof(char));
        client.socket_fd = -1;
        client.XPos = -1;
        client.YPos = -1;
        client.Range=-1;
}
/*
*function to free client after used
*/
void free_client(struct Client client){
    initial_client(client);
}
/**
*determine the socket with the largest int value, the function should take in client 
*which is the array of client structs whose sockets we wish to check
* and return the int value of the largest socket number in the provided array of sockets
*/
int max_socket(const struct Client* clients,int TCP_fd)
{
    int ans = TCP_fd;
    for (int i=0; i<MAX_USER_NUM; ++i)
        if (clients[i].socket_fd > ans)
            ans = clients[i].socket_fd;
    return ans;
}
/**
 *initialize an fd set with a list of ports, as well as stdin
 * the function will take in two parameter:
 * clients is the array of client structs whose ports we wish to listen to in the fd set set
 * TCP_fd is  the fd set to initialize
*/
fd_set selectOnSockets(const struct Client* clients, int TCP_fd)
{
    fd_set set;
    FD_ZERO(&set);
    FD_SET(TCP_fd, &set);
    for (int i=0; i<MAX_USER_NUM; ++i) {
        if (clients[i].socket_fd != -1) {
            FD_SET(clients[i].socket_fd, &set);
        }
    }
    select(max_socket(clients,TCP_fd)+1, &set, NULL, NULL, NULL);
    return set;
}