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

struct Point* initial_point();
int loadPoint(char* message,struct Point** points);
void freePoint(struct Point* point);
void freePoint(struct Point** point, int length);


//load and parse the message, return number of reachable 
int loadPoint(char* message,struct Point** points);

void freePoints(struct Point** points, int leng);

void freePoint(struct Point* point);