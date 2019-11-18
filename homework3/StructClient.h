//struct called client 
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


struct Client{
	char* ID;
	float XPos;
	float YPos;
    int sockfd;
};
void initial_client(struct Client client);
void free_client(struct Client client);
fd_set selectOnSockets(const struct client* clients, int TCP_fd);
//still need a handle client need to implement.  dropbox 里面的这个function不齐全暂时不炼金了
