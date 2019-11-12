//this is file that complement functions in DataMessage.h
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
 struct DataMessage* initMessage(){
 	struct NewMessage*=(struct DataMessage*)malloc(sizeof(struct DataMessage));
 	NewMessage->Message=(char*)malloc(BUFFER_SIZE*sizeof(char));
 	NewMessage->OriginID=(char*)malloc(BUFFER_SIZE*sizeof(char));
 	NewMessage->NextID=(char*)malloc(BUFFER_SIZE*sizeof(char));
 	NewMessage->DestinationID=(char*)malloc(BUFFER_SIZE*sizeof(char));
 	NewMessage->HopListLength=0;
	NewMessage->HopList=(char**)malloc(BUFFER_SIZE*sizeof(char*));
	for (int i = 0; i < 2*BUFFER_SIZE; ++i){
        NewMessage->hopList[i] = (char*)malloc(BUFFER_SIZE*sizeof(char));
	}
	return NewMessage;
 }


 void freeDataMessage(struct DataMessage* Message){

 	free(Message->OriginID);
    free(Message->NextID);
    free(Message->DestinationID);
    for (int i = 0; i < 2*BUFFER_SIZE; ++i){
        free(Message->HopList[i]);
    }
    free(Message->HopList);
    free(Message);
 }