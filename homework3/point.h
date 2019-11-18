
struct Point{
	char* ID;
	float XPos;
	float YPos;
	bool isBaseStation;
};


//load and parse the message, return number of reachable 
int loadPoint(char* message,struct Point** points);

void freePoints(struct Point** points, int leng);

void freePoint(struct Point* point);