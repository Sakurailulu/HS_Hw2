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

struct Point{
	char* ID;
	float XPos;
	float YPos;
	bool isBaseStation;
};
/**
*initialized the point and set memory
*/
struct Point* initial_point(){
	struct Point* point=(Point*)malloc(sizeof(struct Point));
	point->ID=malloc(BUFFER_SIZE*sizeof(char));
	point->XPos=-1;
	point->YPos=-1;
	point->isBaseStation=false;
}
/**
*convert string of reachable to an array of Point
*
*/
int loadNode(char* message,struct Point** points){
//REACHABLE [NumReachable] [ReachableList]([ID] [XPosition] [YPosition]) 
    char word[BUFFER_SIZE];
    int count=0;
    int temp=0;
    int NumReachable=0;
    if(message[strlen(message)-1]!='\n'){
        strcat(message,"\n");
    }
    int curr=0;
    for(int i=0;i<strlen(message);i++){
        if( message[i]==' '|| message[i]=='\n'){
            if(count==0){
                count++;
                continue;
            }
            else if(count==1){
                NumReachable=atoi(word);
                points=(struct Points**)malloc(NumReachable*sizeof(struct Point*));
            }
            else{
                        points[curr] = initial_point();
                        points[curr]
                        curr++;
             
            }
            count++;
            memset(word,0, sizeof(char*));
            temp=0;
           
        }
        else{
            word[temp]=message[i];
            temp++;
        }


    }
}




