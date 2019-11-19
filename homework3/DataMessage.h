//this is struct called DataMessage which is the format for data 
//that passing through clients and base stations
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

struct DataMessage{
 
 char* OriginID;
 char* NextID;
 char* DestinationID;
 int HopListLength;
 char** HopList;
};
struct DataMessage* initial_Message();
void LoadMessage(char* message, struct DataMessage* data);
// load dataMessage from received text
void ConvertMessage(struct DataMessage* data, char* message);
// convert dataMessage to text form
void freeDataMessage(struct DataMessage* data);
