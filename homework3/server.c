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
#include "StructClient.h"

#define BUFFER_SIZE 1024
#define MAX_USER_NUM 64
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
  * free the memory of Base station
  * @param station
  */
void freeStation(struct BaseStation* station){
    initial_Station(station);
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
*helper function to calculate the distance between two point 
*such as distance between two sensors and  between sensor and base station
**/
float Cal_distance(float x1, float y1,float x2,float y2){
return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}



int sendData(char* command, int NumStations, struct BaseStations* BaseStations, struct Client* clients, struct DataMessage* message){
    
    // return 0 if control --> client
    // return 1 if base --> client
    // return 2 if base --> base
    // return 3 if cannot find next legal client
    int flag= 0;
    char word[BUFFER_SIZE];
    char origin[BUFFER_SIZE];
    char destination[BUFFER_SIZE];
    int count=0;
    int temp=0;
    if(command[strlen(command)-1]!='\n'){
        strcat(command,"\n");
    }
    int curr=0;
    for(int i=0;i<strlen(command);i++){
        if( command[i]==' '|| command[i]=='\n'){
            if(count==0){
                if(strcmp(word,"SENDDATA")!=0){
                    flag=3;
                    return flag;
                }
            }
            else if(count==1){
                strcpy(origin,word);
            }
            else if(count==2){
                strcpy(destination,word);
            }
            count++;
            memset(word,0, sizeof(char*));
            temp=0;           
        }
        else{
            word[temp]=command[i];
            temp++;
        }
    }
    strcpy(message->OriginID,origin);
    strcpy(message->DestinationID,destination);
    strcpy(message->NextID,"default");
    if (strcmp(origin, "CONTROL") == 0){
        flag= 0;
        message->HopListLength = 0;
    }else{
         flag= 1;
        for (int i = 0; i < NumStations; ++i){
            if (strcmp(dest,BaseStations[i]->ID) == 0){
                flag= 2;
            }
        }
        strcpy(message->HopList[message->HopListLength], origin);
        message->HopListLength ++;
    }

    float destinationX = 0;
    float destinationY = 0;
    float minDistance = 999999;
    char Nextid[BUFFER_SIZE];
    for (int i = 0; i < NumStations; i++){
        if (strcmp(destination, BaseStations[i]->ID) == 0){
            destinationX = BaseStations[i]->XPos;
            destinationY = BaseStations[i]->YPos;
        }
    }

    for (int i = 0; i < MAX_USER_NUM; ++i){
        if (strcmp(destination, clients[i].ID) == 0){
            destinationX = clients[i].XPos;
            destinationY = clients[i].YPos;
        }
    }

    bool NextBase = false;

    if (flag == 0){
        // if origin is CONTROL, find the closest base to dest
        for (int i = 0; i < NumStations; i++){
            float currDistance = Cal_distance(BaseStations[i]->XPos, destinationX,BaseStations[i]->YPos,destinationY);
            if (currDistance < minDistance){
                NextBase= true;
                minDisance = currDistance;
                strcpy(Nextid, BaseStations[i]->ID);
            }
        }
        strcpy(message->NextID, Nextid);
    }else {
        // otherwise get the (x, y) of origin first, then find the closest reachable
        // node to dest
        float OriginX = 0;
        float OriginY = 0;
        // for each linked base of origin base
        for (int i = 0; i < NumStations; ++i){
            if (strcmp(origin, BaseStations[i]->ID) == 0){
                // compare distance from each linked base to dest
                // the shortest one is the next base
                OriginX = BaseStations[i]->XPos;
                OriginY = BaseStations[i]->YPos;
                for(int j = 0; j < BaseStations[i]->NumLinks; j++){
                    for (int k = 0; k < NumStations; ++k){
                        if (strcmp(BaseStations[k]->ID, BaseStations[i]->ListofLinks[j]) == 0){
                            float currDistance = Cal_distance(BaseStations[k]->XPos,destinationX, BaseStations[k]->YPos,destinationY);
                            if(currDistance < minDistance){
                                NextBase = true;
                                minDistance = currDistance;
                                strcpy(Nextid, BaseStations[k]->ID);
                            }
                        }
                    }
                }
            }
        }
        // then for each reachable client sensor in range:
        for (int i = 0; i < MAX_USER_NUM; i++){
            if (clients[i].socket_fd != -1 && clients[i].ID[0] != '\0'){
                if(Cal_distance(clients[i].XPos, clients[i].YPos, OriginX, OriginY)
                   <= clients->Range){
                    // if current sensor legal and in range
                    float currDistance = Cal_distance(clients[i].XPos,destinationX, clients[i].yPos,destinationY);
                    if (currDistance < minDistance){
                        NextBase = 0;
                        minDistance = currDistance;
                        strcpy(Nextid, clients[i].ID);
                    }
                }
            }
        }
        strcpy(message->NextID, Nextid);
    }
    
    // handle internal message transprot
    char currentBase[BUFFER_SIZE];
    int Flag = 1;
    while(NextBase){
        strcpy(currentBase, message->NextID);


        float OriginX = 0;
        float OriginY = 0;
        // for each linked base of origin base
        for (int i = 0; i < NumStations; ++i){
            if (strcmp(origin, BaseStations[i]->ID) == 0){
                // compare distance from each linked base to dest
                // the shortest one is the next base
                OriginX = BaseStations[i]->XPos;
                OriginY = BaseStations[i]->YPos;
                for(int j = 0; j < BaseStations[i]->NumLinks; j++){
                    for (int k = 0; k < NumStations; ++k){
                        if (strcmp(BaseStations[k]->ID, BaseStations[i]->ListofLinks[j]) == 0){
                            float currDistance = Cal_distance(BaseStations[k]->XPos,destinationX, BaseStations[k]->YPos,destinationY);
                            int legal = 1;
                            // if this base not in hopList
                            for (int l = 0; l < message->HopListLength; l++){
                                if (strcmp(message->HopList[l], BaseStations[l]->ID) == 0){
                                    legal = 0;
                                }
                            }


                            if(currDistance < minDistance && legal){
                                Flag = 0;
                                NextBase = 1;
                                minDistance = currDistance;
                                strcpy(Nextid, BaseStations[k]->ID);
                            }
                        }
                    }
                }
            }
        }


        // then for each reachable client sensor in range:
        for (int i = 0; i < MAX_USER_NUM; ++i){
            if (clients[i].socket_fd != -1 && clients[i].ID[0] != '\0'){
                if(Cal_distance(clients[i].XPos,OriginX, clients[i].YPos, OriginY)
                    <= clients->Range){
                    // if current sensor legal and in range(not in the hopList)
                    int legal = 1;
                    for (int l = 0; l < message->HopListLength; ++l){
                        if (strcmp(message->HopList[l], clients[i].ID) == 0){
                                legal = 0;
                            }
                        }

                    float currDist = Cal_distance(clients[i].XPos, destinationX, clients[i].YPos, destinationY);
                    if (currDistance < minDistance && legal){
                        Flag = 0;
                        NextBase = 0;
                        minDistance = currDistance;
                        strcpy(Nextid, clients[i].ID);
                    }
                }
            }
        }






        if(Flag){
            printf("%s: Message from %s to %s could not be delivered.\n",
                    currentBase, message->OriginID, message->DestinationID);
            Flag = 0;
            break;
        }
        else{    
            printf("%s: Message from %s to %s being forwarded through %s\n",
                    currentBase, message->OriginID, message->DestinationID, currentBase);
            strcpy(message->NextID, Nextid);
            strcpy(message->HopList[message->HopListLength], currentBase);
            message->HopListLength++;
        }
    }

    return Flag;
}

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
    /*
    for(int i=0;i<NumStations;i++){
        printBase(&BaseStations[i]);
    }
    */
    /* Create the listener socket as TCP socket (SOCK_STREAM) */
    struct sockaddr_in server;
    int serverSd = initServer(&server, port);
 
    // array of client structure
    struct Client* clients = (Client*)malloc(MAX_USER_NUM * sizeof(struct Client));
    for (int i = 0; i < MAX_USER_NUM; ++i){
        initial_client(clients[i]);
    }

    unsigned int length = sizeof(struct sockaddr_in);
 
    int child_pid = fork();
    if(child_pid==0){
        //use child to handle the process for TCP server
        while(1){
             fd_set Clients_fd = selectOnSockets(clients, serverSd);
             bool find=false;
            if (FD_ISSET(serverSd, &fdClients)){
                for(int i=0;i<MAX_USER_NUM;i++){
                    if(clients[i].socket_fd<0){
                        clients[i].socket_fd=accept(serverSd, (struct sockaddr *) &server,&sizeof(struct sockaddr_in));
                        if(clients[i].socket_fd<0){
                            fprintf(stderr,"ERROR: Accept failed\n");
                        }
                        clients[i].id[0] = '\0';
                        break;
                    }
                }
                if(!find){
                    printf("Current client array is full\n");
                }

            }
        }
        for (int i = 0; i < MAX_USER_NUM; i++){
            if(clients[i]->socket_fd!=-1&&FD_ISSET(clients[i]->socket_fd, &fdset)){
               // handle each connection
                printf("CONTROL: clientSd is %d\n", currentClient->clientSd);
                //###############################################################
                //###############################################################
                //待补充
            }
        }


    }

    
    else{
        //use the parent to handle the output to terminal
        char command[BUFFER_SIZE];
        while(TRUE){
              printf("Please Enter command here: ");
            if(fgets(command, sizeof(command), stdin) == NULL){
                //continue getting command from stdin
                continue;
            }
            command[strlen(command)-1] = '\0';
            //printf("Entered command: %s\n", command);
            if(strcmp(command, "QUIT") == 0){
                printf("CONTROL: terminate tcp server on %d\n", child_pid);
                kill(child_pid,SIGKILL);
                printf("CONTROL: terminate the main process\n");
                break;
            }
            // other commands:
            else{
                struct DataMessage* message = initial_Message();
                int success = sendData(command, NumStations, BaseStations, clients, message);
                if(success == 0){
                    printf("CONTROL: Invalid Command\n");
                }else{
                    char* Message=malloc(BUFFER_SIZE*sizeof(char));
                    ConvertMessage(message, Message);
                    printf("NEW DataMessage:\n\t%s\n",Message);
                }

            }
        }

    }
 




    
     for(int i=0;i<NumStations;i++){
        freeStation(&BaseStations[i]);
     }
     free(BaseStation);
     for(int i=0;i<MAX_USER_NUM;i++){
        free_client(clients[i]);
     }
     free(clients);

     
    return 0;

}
