//this is the .h file for struct called BaseStation
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
#define BUFFER_SIZE 1024
struct BaseStation{
	char[BUFFER_SIZE] ID;
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
	memset(client->id,0, sizeof(client->id));
	XPos=0;
	YPos=0;
	NumLinks=0;
	ListofLinks=malloc(102400*sizeof(char*));
	visited=false;
}
/**
*function to find certain basestation with given ID
**/
struct BaseStation findStation(char* id,struct BaseStation* Stations,int length){
for(int i=0;i<length;i++){
	if(strcmp(id,Stations[i].id)==0){
		return Stations[i];
	}
}

}
