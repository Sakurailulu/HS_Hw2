//assignment3 for network programming、
//this is the server for assignment3

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

#include "BaseStation.h"
#define BUFFER_SIZE 1024
/**
 * build the structure as BaseStation
 */
struct BaseStation{
    char[BUFFER_SIZE] ID;
    float XPos;
    float YPos;
    int NumLinks;
    char** ListofLinks;
    bool visited;
};
/**
 * setup the tcp socket and binding. This function will
 *fprintf any error which happened during the creating and binding
 * and the function will return the sockfd
 */
int Set_Socket(int port){
    //create the socket and return error message if socket creation failed
    int sockfd;
    struct sockaddr_in server;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        fprintf(stderr,"ERROR: socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( port );
    //bind the socket and return error message if socket binding failed
    if(bind(sockfd, (struct sockaddr *)&server, sizeof(server))<0){
        fprintf(stderr,"ERROR: socket bining failed\n");
        exit(EXIT_FAILURE);
    }
    /*
    //listen for connection and return error message if any error occur
    if (listen(sockfd,1) < 0) {
        perror("ERROR: socket listening failed\n");
        exit(EXIT_FAILURE);
    }
     */
    return sockfd;
}
/**
 * help function to read in Base station file which is argv[2], create the BaseStations and store in
 * an array of BaseStations.
 * the DATAMESSAGE [OriginID] [NextID] [DestinationID] [HopListLength] [HopList]
 * this function will return an integer which is the size of BaseStations
 */
int ReadStation(char* file,struct BaseStation[] BaseStations){
    FILE* fp = fopen(file, "r");
    if(!fp){
        perror("ERROR: Fail to open file\n");
        exit(EXIT_FAILURE);
    }

    char[BUFFER_SIZE] line;
    int index=0
    while (fgets(line,BUFFER_SIZE,fp)) {
       BaseStations[index]=LoadStation(line);
       index++;
    }
    fclose(fp);
    return index;
 }
/**
 * helper function to store information from line into basestation
 */
 struct BaseStation* LoadStation(char* line){
    struct BaseStation* station = (struct *BaseStation)malloc(sizeof(struct BaseStation));
    initial_Station(station);
    char* word;
    int count=0;
    for(int i=0;i<strlen(line);i++){
        if(line[i]==" "||line[i]=="\n"){
            if(count==0){
                station->ID=word;
            }
            else if(count==1){
                station->XPos=atof(word);
            }
            else if(count==2){
                station->YPos=atof(word);
            }
            else if(count==3){
                station->NumLinks=atoi(word);
                station->ListofLinks=(char**)malloc(station->NumLinks*sizeof(char*));
            }
            else{
                for(int j=0;j<station->NumLinks;j++){
                    if(!station->ListofLinks[j]){
                        station->ListofLinks[j]=word;
                    }
                }
            }
            count++;
            memset(word,0, sizeof(word));
        }
        else{
            strcat(word,&line[i],1);
        }
    }
    return station;
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
    for (int i = 0; i < station->NumLinks; ++i){
        printf("\t\t%s\n", station->ListofLinks[i]);
    }
    printf("\n");
}

/**
*helper function to calculate the distance between two point 
*such as distance between two sensors and  between sensor and base station
**/
float Cal_distance(float x1, float y1,float x2,float y2){
return sqrt((x1-x2)^2+(y1-y2)^2);
}

int main(int argc,char* argv[]){
	struct BaseStation[] BaseStations;
	int port=atoi(argv[1]);
	char* file=argv[2];
	int socket_fd=Set_Socket(port);
	int NumStations=ReadStation(file,BaseStations);

}