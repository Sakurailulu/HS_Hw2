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
struct Point** loadPoint(char* message,int* NumReachable);
void freePoint(struct Point* point);
void freePoints(struct Point** point, int length);



