//this is file that complement functions in DataMessage.h

#include "DataMessage.h"

/**
*function to initialized the message so that we can modify the varibles in the function later 
*without any uninitialized error
*/
 struct DataMessage* initial_Message(){
 	struct DataMessage* NewMessage=(struct DataMessage*)malloc(sizeof(struct DataMessage));
 	NewMessage->OriginID=(char*)malloc(BUFFER_SIZE*sizeof(char));
 	NewMessage->NextID=(char*)malloc(BUFFER_SIZE*sizeof(char));
 	NewMessage->DestinationID=(char*)malloc(BUFFER_SIZE*sizeof(char));
 	NewMessage->HopListLength=0;
	NewMessage->HopList=(char**)malloc(BUFFER_SIZE*sizeof(char*));
	for (int i = 0; i < 2*BUFFER_SIZE; ++i){
        NewMessage->HopList[i] = (char*)malloc(BUFFER_SIZE*sizeof(char));
	}
	return NewMessage;
 }
/*
*function to load an initialized data message from message
*/
 void LoadMessage(char* message, struct DataMessage* dataMessage){   
    char* word=(char*)malloc(BUFFER_SIZE*sizeof(char));
    int count=0;
    int temp=0;
    if(message[strlen(message)-1]!='\n'){
        strcat(message,"\n");
    }
    int curr=0;
    for(int i=0;i<strlen(message);i++){
        if( message[i]==' '|| message[i]=='\n'){
            if(count==0){
            count++;
            memset(word,0, BUFFER_SIZE*sizeof(char));
            temp=0;
                continue;
            }
            else if(count==1){
                strcpy(dataMessage->OriginID,word);
            }
            else if(count==2){
                strcpy(dataMessage->NextID,word);
            }
            else if(count==3){
                strcpy(dataMessage->DestinationID,word);    
            }
            else if(count==4){
                dataMessage->HopListLength=atoi(word);
                dataMessage->HopList=(char**)malloc(dataMessage->HopListLength*sizeof(char*));
            }
            else{
                        dataMessage->HopList[curr] = (char*)malloc(sizeof(char));
                        strcpy( dataMessage->HopList[curr],word);
                        curr++;
             
            }
            count++;
            memset(word,0, BUFFER_SIZE*sizeof(char));
            temp=0;
           
        }
        else{
            word[temp]=message[i];
            temp++;
        }


    }
    free(word);

    
 
}
/*
*function to convert datamessage to a string of message for 
* output purpose
*/
void ConvertMessage(struct DataMessage* dataMessage, char* message){
    sprintf(message, "DATAMESSAGE %s %s %s %d ",
            dataMessage->OriginID, dataMessage->NextID, 
            dataMessage->DestinationID, dataMessage->HopListLength);
 
    // convert HopList to text
    char* Buffer=(char*)malloc(BUFFER_SIZE*sizeof(char));
    strcpy(Buffer, dataMessage->HopList[0]);
    for (int i = 1; i < dataMessage->HopListLength; ++i){
        strcat(Buffer, " ");
        strcat(Buffer, dataMessage->HopList[i]);
    }
    strcat(message, Buffer);
}
/*
*helper function to free the memeory in datamessage
*/

 void freeDataMessage(struct DataMessage* Message){

 	free(Message->OriginID);
    free(Message->NextID);
    free(Message->DestinationID);
    for (int i = 0; i < Message->HopListLength; ++i){
        free(Message->HopList[i]);
    }
    free(Message->HopList);
    free(Message);
 }