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
    for (int i = 0; i < station->NumLinks; ++i){
        printf("\t\t%s\n", station->ListofLinks[i]);
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
    /*
    int sockfd;
    struct sockaddr_in server;
    sockfd=socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);;
    if(sockfd<0){
        fprintf(stderr,"ERROR: socket creation failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Socketfd: %d \n",sockfd);
     bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( port );
    printf("Socketfd: %d \n",sockfd);
    //bind the socket and return error message if socket binding failed
    if(bind(sockfd, (struct sockaddr *)server, sizeof(server))<0){
    	
        fprintf(stderr,"ERROR: socket binding failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Tanjiro\n");


    

    
    //listen for connection and return error message if any error occur
    if (listen(sockfd,5) < 0) {
        perror("ERROR: socket listening failed\n");
        exit(EXIT_FAILURE);
    }
    
    return sockfd;
    */
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
    printf("line 114 is good\n");
    initial_Station(&station);
    //printf("line 116 is good\n");
    char word[BUFFER_SIZE];
    int count=0;
    int temp=0;
/*
    for(int j=0;j<strlen(line);j++){
        if(temp == 0){
            printf("%s\n",&line[j]);
            temp++;
        }

    }
    */
printf("current line: %s\n",line);
    for(int i=0;i<strlen(line);i++){
        //printf("line 120 is good\n");

        if( line[i]==' '|| line[i]=='\n'){
            //printf("%s\n",&line[i]);
            //printf(" word %s, i is %d\n",word,i);
            
            if(count==0){
            	printf("count==0: word %s\n",word);
                strcat(station.ID,word);
            }
            else if(count==1){
            	printf("count==1: word %s\n",word);
                station.XPos=atof(word);
            }
            else if(count==2){
            	printf("count==2: word %s\n",word);
                station.YPos=atof(word);
            }
            else if(count==3){
            	printf("count==3: word %s\n",word);
                station.NumLinks=atoi(word);
                station.ListofLinks=(char**)malloc(station.NumLinks*sizeof(char*));
            }
            else{
            	printf("count==4: word %s\n",word);
                for(int j=0;j<station.NumLinks;j++){
                    if(!station.ListofLinks[j]){
                        station.ListofLinks[j]=word;
                    }
                }
                printf("I am here\n");
            }
            count++;
            //printf("line 144 is good\n");
            memset(word,0, sizeof(char*));
            temp=0;
            printf("line 146 is good\n");
        }
        else{
            //printf("line 149 is good\n");
            //printf("%s\n",word);
            //printf("%s\n",&line[i]);
            word[temp]=line[i];
            temp++;
            //printf("current word is %s\n",word);
            //printf("line 153 is good\n");
        }


    }
    printf("Right before return\n");
    printBase(&station);
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
    printf("line 163 is good\n");
    while (fgets(line,BUFFER_SIZE,fp) != NULL) {
        printf("line 165 is good\n");
        BaseStations[index]=LoadStation(line);
        printf("line 167 is good\n");
        index++;
    }
    fclose(fp);
    printf("line 170 is good\n");
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
	struct BaseStation* BaseStations;
	int port=atoi(argv[1]);
	char* file=argv[2];
	int socket_fd=Set_Socket(port);
	printf("Set socket\n");
	int NumStations=ReadStation(file,BaseStations);
	return 0;

}