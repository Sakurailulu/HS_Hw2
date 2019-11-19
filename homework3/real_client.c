#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "point.h"
#include "StructClient.h"
#include "DataMessage.h"
#define MAX 1024
//from our lab6 code


struct addrinfo hints, *infoptr; // So we don't need to use memset global variables

/**
*helper function to calculate the distance between two point 
*such as distance between two sensors and  between sensor and base station
**/
float Cal_distance(float x1, float y1,float x2,float y2){
return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

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
  strcpy(client_Id,argv[3]);
	int port = atoi(argv[2]);
	float client_Range = atoi(argv[4]);
	float client_XPos = atoi(argv[5]);
	float client_YPos = atoi(argv[6]);
	
	//find ip address
	char hostAddr[256];
	find_IP(hostAddr,argv[1]); //**************

	//initialize client socket
	int sockfd;
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))== -1){
		perror("Create socket failed\n");
		exit(EXIT_FAILURE);//********
	}//******could add else statement when create server successful

	struct sockaddr_in clientaddr;
	bzero(&clientaddr, sizeof(clientaddr));
	// link port, addr
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(port); 
    servaddr.sin_addr.s_addr = inet_addr(hostAddr); 

  	if ( connect(sockfd, (struct sockaddr *)&clientaddr, sizeof(clientaddr)) != 0 ){
        perror("Connect to server failed.\n");
        exit(EXIT_FAILURE);
    }


    char buffer[MAX];
    char initial_msg[MAX];
    int flag;
    
    strcpy(initial_msg,client_Id);
    strcat(initial_msg," ");
    strcat(initial_msg, client_Range);
    strcat(initial_msg, " ");
    strcat(initial_msg, client_XPos);
    strcat(initial_msg, " ");
    strcat(initial_msg, client_YPos);
    printf("%s\n", initial_msg);
    bzero(buffer, sizeof(buffer));

    read(sockfd,buffer,sizeof(buffer));
    write(sockfd,client_Range,sizeof(client_Range));
    bzero(buffer, sizeof(buffer)); 

    read(sockfd,buffer,sizeof(buffer));
    write(sockfd,client_XPos,sizeof(client_XPos));
    bzero(buffer, sizeof(buffer)); 
    read(sockfd,buffer,sizeof(buffer));
    write(sockfd,client_YPos,sizeof(client_YPos));
    bzero(buffer, sizeof(buffer)); 
    read(sockfd,buffer,sizeof(buffer));


    pid_t pid = fork();
    if(pid == 0){//send to server
    	while(1){
    		bzero(&buffer, sizeof(buffer)); //******&****
    		//read
    		flag = read(sockfd, buffer, sizeof(buffer));
    		if (flag < 10){//*************
    			continue;
    		}
    		printf("%s\n", buffer);
    		char* current_datamsg[MAX];
    		struct DataMessage* new_msg = initial_Message();
    		LoadMessage(buffer,new_msg);
    		//check if the clientid is the same as destinationid
    		//which means we are at the destination now
    		if(strcmp(client_Id,new_msg->DestinationID)==0){
    			printf("%s: Message from %s to %s successfully received.\n",client_Id, new_msg->OriginID, client_Id);
                freeDataMessage(new_msg);
    		}else{//we are not at the position we want, need to forward and SENDDATE
    			printf("%s: Message from %s to %s being forwarded through %s\n", client_Id,new_msg->OriginID, new_msg->DestinationID,client_Id);
    			//set the buffer to ZERO need to call the WHERE command
                bzero(buffer, sizeof(buffer));
                Where_command(buffer, new_msg->DestinationID);
                flag = write(sockfd, buffer, sizeof(buffer));
                if(flag == 1)
                	;//************
                bzero(buffer, sizeof(buffer));

                flag = read(sockfd, buffer, sizeof(buffer));
                //get the x and y from the line*********
                char word[MAX];
                int count = 0;
                int temp = 0;
                int getX,getY;
               	for(int i=0;i<strlen(line);i++){
               		if(line[i] == ' ' || line[i] == '\n'){
               			if(count == 1){
               				getX = atof(word);
               			}else if(count ==2){
               				getY = atof(word);
               			}
               		count++;
               		temp = 0;
               		}else{
               			word[temp]=line[i];
            			temp++;
               		}
               		
               	}//********************************
               	UpdatePosition_command(buffer, client_Id, client_Range,client_XPos,client_YPos);
               	flag = write(sockfd, buffer, sizeof(buffer));
               	if (flag == 1)
               		;
               	bzero(buffer, sizeof(buffer));
               	struct Point** all_reachable;
               	// receive message from server (REACHABLE #Reachable client1 3 3 client2 4 4)
               	flag = read(sockfd, buffer, sizeof(buffer));
               	//***********get the number of reachable node
               	//since our LOADPOINT returns an array of point
               	//further implement this part !!!*************

               	int num_recheable = 0;
               	float get_small = 99999;
               	bool tag = true;
               	int all_valid = 0;
               	char* temp;
               	for(int i =0;i<num_recheable;i++){
                    for(int j=0;j<tempMsg->HopListLength;j++){
                        if(strcmp(all_reachable[i]->ID,new_msg->HopList[j]) == 0){
                            // check if the point is in the HopList
                            tag = false;
                        }
                    }
                    if(tag == true && (Cal_distance(getX,getY,all_reachable[i]->XPos,all_reachable[i]->YPos) < get_small)){
                    	all_valid++; 
                        strcpy(temp,reachable[i]->ID); //set the temp node ID to the i th in the reachable list
                        //set the small value to the distance between sensor and the i th in the rechable list
                        get_small = Cal_distance(getX,getY,all_reachable[i]->XPos,all_reachable[i]->YPos);
                     
                    }
                }
                //if we go thru the reachabel list and cannot find one node to go to
                if(all_valid == 0){
                	printf("%s: Message from %s to %s could not be delivered.\n",client_Id,
                	new_msg->OriginID,new_msg->DestinationID);
                	continue;
                }

                new_msg->HopListLength ++;
                strcpy(new_msg->HopList[new_msg->HopListLength], client_Id);
                strcpy(new_msg->NextID,temp);
              
                convertDataMessage(new_msg,buffer);
                bzero(buffer, sizeof(buffer)); 
                write(sockfd,buffer,sizeof(buffer));
    		}
    		//free the memory for datamsg
    		freeDataMessage(new_msg);

    	}
    }
    else{//receive from server
 			while(1){
 				bzero(buffer, sizeof(buffer));
            	printf("Enter the string :"); 
            	fgets(buffer, 10*MAX, stdin);
            	char* cur_command;

            	//get the command from the command line
            	//need to implement 	

            	if(strcmp(cur_command, "SENDDATA") == 0){
            		bzero(buffer, sizeof(buffer));
            	}else if(strcmp(cur_command, "MOVE") == 0){
            		bzero(buffer, sizeof(buffer));
            	}else if (strcmp(cur_command, "QUIT") == 0){
            		return 0;
            		//need to free the memory 
            	}

            	//need to free the memory
 			}
		}

		return 0;

}





















