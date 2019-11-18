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

struct PointOnGraph{
	char[BUFFER_SIZE] ID;
	float XPos;
	float YPos;
	bool isBaseStation;
};


// void print_points(struct PointOnGraph* all_the_points, int total_number_of_points){
// 	for(int i=0; i<total_number_of_points; i++){
// 		printf("At location %d  %d   is  a ", all_the_points[i]->XPos,all_the_points[i]->YPos);
// 		if(all_the_points[i]->isBaseStation == true){
// 			printf("basestation\n" );
// 		}else{
// 			printf("censor\n");
// 		}
// 	}
// }

// void initialize_point(struct BaseStation* Station, struct Client* Client){
// 	if(Station == NULL){
// 		struct PointOnGraph new_point;
// 		memset(Client->ID,0, sizeof(Client->ID));
// 		new_point->ID = Client->ID;
// 		new_point->XPos = Client->XPos;
// 		new_point->YPos = Client->YPos;
// 		isBaseStation = false;
// 	}else if(Client == NULL){
// 		struct PointOnGraph new_point;
// 		memset(Station->ID,0, sizeof(Station->ID));
// 		new_point->ID = Station->ID;
// 		new_point->XPos = Station->XPos;
// 		new_point->YPos = Station->YPos;
// 		isBaseStation = true;

// 	}else{
// 		bool tag = false;
// 		//neither is null, idk problem i guess;
// 	}
// }
// void set_point_ID(char* _id){
// 	ID = _id;
// }
// void set_point_X(float _x){
// 	XPos = _x;
// }
// void set_point_Y(float _y){
// 	YPos = _y;
// }
// //this is to set if the struct passed in is a base station ,= true
// //if the struct is not a base station, it's a sensor ,= false
// void set_is_base_staiton(bool _setbool){
// 	isBaseStation = _setbool;
// }

//struct PointOnGraph* all_points(struct BaseStation* Station,)




