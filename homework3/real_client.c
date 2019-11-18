#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define MAX 1024
//from our lab6 code


struct addrinfo hints, *infoptr; // So no need to use memset global variables


void find_IP(char* host, char* hostname){
	char* test_var = hostname;
	int result = getaddrinfo(test_var, NULL, &hints, &infoptr);

    if (result) {
        fprintf(stderr, "getaddrinfo error code: Name or service not known\n");
        fprintf(stderr,"getaddrinfo failed error: Bad message\n");
        exit(1);
    }

    char Ip_address[256];
    struct  addrinfo *p;
    	
    for (p = infoptr; p != NULL; p = p-> ai_next){
    	getnameinfo(p->ai_addr, p->ai_addrlen, Ip_address, sizeof (Ip_address), NULL, 0, NI_NUMERICHOST);
    }
    freeaddrinfo(infoptr);
    strcpy(host,Ip_address);
}

//handle the quit command from the client
void Quit_command(char* temp){
	strcpy(temp, "QUIT");
}

//handle the update position command from the client
//UPDATEPOSITION  SensorID] [SensorRange] [CurrentXPosition] [CurrentYPosition]
void UpdatePosition_command(char* temp, char* Id, float SensorRange, float CurXPos, float CurYPos){
	char str_range[MAX];
	char str_X[MAX];
	char str_Y[MAX];
	//convert float to string, buffer size MAX set to 128
	gcvt(SensorRange,2,str_range);
	gcvt(CurXPos,2,str_X);
	gcvt(CurYPos,2,str_Y);
	strcpy(temp, "UPDATEPOSITION ");
	strcat(temp, Id);
	strcat(temp, " ");
	strcat(temp, str_range);
	strcat(temp, " ");
	strcat(temp, str_X);
	strcat(temp, " ");
	strcat(temp, str_Y);
}

//handle the move command from the client
//MOVE [NewXPosition] [NewYPosition] 
void Move_command(char* temp, float CurXPos, float CurYPos){
	char str_X[MAX];
	char str_Y[MAX];
	sprintf(str_X, "%f", CurXPos);
	sprintf(str_Y, "%f", CurYPos);
	strcpy(temp, "MOVE "); //********************
	strcat(temp, str_X);
	strcat(temp, " ");
	strcat(temp, str_Y);
}

//handle the where command to get the location of a particular base station or sensor ID 
//from the control server
//WHERE [SensorID/BaseID]
void Where_command(char* temp, char* TargetId){
	strcpy(temp, "WHERE ");
	strcat(temp, TargetId);
}


int main(int argc, char *argv[]){
	char client_Id[MAX];
	int port = atoi(argv[2]);
	char* client_Id = argv[3]; //**************
	float client_Range = atoi(argv[4]);
	float client_XPos = atoi(argv[5]);
	float client_YPos = atoi(argv[6]);
	
	//find ip address
	char hostAddr[256];
	find_IP(hostAddr,argv[1]); //**************

	//initialize client socket
	int sockfd;
	




}





















