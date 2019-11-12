//assignment3 for network programming、
//this is the server for assignment3

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
/**
 * build the structure as BaseStation
 */
struct BaseStation{
    char ID[BUFFER_SIZE];
    float XPos;
    float YPos;
    int NumLinks;
    char** ListofLinks;
    bool visited;
};

struct Client{
	char* ID;
	float XPos;
	float YPos;
	char* Buffer;
	struct DataMessage* Message;
};

/**
*initialized the BaseStation
*/
void initial_Station(struct BaseStation* Station){
	memset(Station->ID,0, sizeof(Station->ID));
	Station->XPos=0;
	Station->YPos=0;
	Station->NumLinks=0;
	Station->ListofLinks=malloc(102400*sizeof(char*));
	Station->visited=false;
}

 /**
  * free the memory of Base station
  * @param station
  */
void freeStation(struct BaseStation* station){
    initial_Station(station);
}
/**
 * print out all the information in given Base station
 * @param base station
 */
void printBase(struct BaseStation* station){
    printf("BaseStation with ID[%s]\n", station->ID);
    printf("\tlocate at (%f, %f)\n", station->XPos, station->YPos);
    printf("\twith %d linked bases\n", station->NumLinks);
    for (int i = 0; i < station->NumLinks; i++){
    	//printf("i %d\n",i);

        printf("%s\n", station->ListofLinks[i]);
    }
    printf("\n");
}
/**
 * setup the tcp socket and binding. This function will
 *fprintf any error which happened during the creating and binding
 * and the function will return the sockfd
 */
int Set_Socket(int port){
    //create the socket and return error message if socket creation failed
     struct sockaddr_in server;
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( port );
    int sock;
    sock=socket(AF_INET, SOCK_STREAM, 0);
    if(sock<0){
        perror("MAIN: ERROR while creating TCP socket");
        exit(EXIT_FAILURE);
    }
    int on = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    
    if(bind(sock, (struct sockaddr *)&server, sizeof(server))<0){
        perror("MAIN: ERROR while binding tcp port");
        exit(EXIT_FAILURE);
    }
    //listen for all 32 clients
    if (listen(sock, 10) < 0) {
        perror("MAIN: ERROR while listening tcp port");
        exit(EXIT_FAILURE);
    }
    return sock;
}
/**
 * helper function to store information from line into basestation
 */
 struct BaseStation LoadStation(char* line){
    struct BaseStation station; 
    initial_Station(&station);
    char word[BUFFER_SIZE];
    int count=0;
    int temp=0;
    if(line[strlen(line)-1]!='\n'){
    	strcat(line,"\n");
    }
    int curr=0;
    for(int i=0;i<strlen(line);i++){
        if( line[i]==' '|| line[i]=='\n'){
            if(count==0){
                strcpy(station.ID,word);
            }
            else if(count==1){
                station.XPos=atof(word);
            }
            else if(count==2){
                station.YPos=atof(word);
            }
            else if(count==3){
                station.NumLinks=atoi(word);
                station.ListofLinks=(char**)malloc(station.NumLinks*sizeof(char*));
            }
            else{
                    	station.ListofLinks[curr] = (char*)malloc(sizeof(char));
                        strcpy(station.ListofLinks[curr],word);
                        curr++;
             
            }
            count++;
            memset(word,0, sizeof(char*));
            temp=0;
           
        }
        else{
            word[temp]=line[i];
            temp++;
        }


    }
    return station;
 }
/**
 * help function to read in Base station file which is argv[2], create the BaseStations and store in
 * an array of BaseStations.
 * the DATAMESSAGE [OriginID] [NextID] [DestinationID] [HopListLength] [HopList]
 * this function will return an integer which is the size of BaseStations
 */
int ReadStation(char* file,struct BaseStation* BaseStations){
    FILE* fp = fopen(file, "r");
    if(!fp){
        perror("ERROR: Fail to open file\n");
        exit(EXIT_FAILURE);
    }

    char line[BUFFER_SIZE];
    int index=0;
    while (fgets(line,BUFFER_SIZE,fp) != NULL) {
        BaseStations[index]=LoadStation(line);
        index++;
    }
    fclose(fp);
    return index;
 }


/**
*helper function to calculate the distance between two point 
*such as distance between two sensors and  between sensor and base station
**/
/*
float Cal_distance(float x1, float y1,float x2,float y2){
return sqrt((x1-x2)^2+(y1-y2)^2);
}
*/
int main(int argc,char* argv[]){
    // prepare base info
    if (argc != 3){
        perror("ERROR: Wrong arguments\n");
        perror("./server.out <controlPort> <baseFile>\n");
        exit(EXIT_FAILURE);
    }
	struct BaseStation* BaseStations=(struct BaseStation*)malloc(64 * sizeof(struct BaseStation));
	int port=atoi(argv[1]);
	char* file=argv[2];
	int socket_fd=Set_Socket(port);
	int NumStations=ReadStation(file,BaseStations);
	for(int i=0;i<NumStations;i++){
		printBase(&BaseStations[i]);
	}




	// for(int i=0;i<NumStations;i++){
	//  	freeStation(&BaseStations[i]);
	//  }
	return 0;

}