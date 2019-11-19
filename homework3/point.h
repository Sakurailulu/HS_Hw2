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

struct Point{
	char* ID;
	float XPos;
	float YPos;
	bool isBaseStation;
};

struct Point* initial_point();
int loadPoint(char* message,struct Point** points);
void freePoint(struct Point* point);
void freePoints(struct Point** points, int length);



