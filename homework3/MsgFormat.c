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

struct MsgFormat{
	char[BUFFER_SIZE] Origin_ID;
	char[BUFFER_SIZE] Next_ID;
	char[BUFFER_SIZE] Dest_ID;
	int hop_list_length;
	char** ListofLinks;
	
};