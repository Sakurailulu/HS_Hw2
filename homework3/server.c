//assignment3 for network programming、
//this is the server for assignment3

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

/**
 * setup the tcp socket and binding. This function will
 *fprintf any error which happened during the creating and binding
 * and the function will return the sockfd
 */
int Set_TCP_Socket(int port){
    //create the socket and return error message if socket creation failed
    int sockfd;
    struct sockaddr_in server;
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        fprintf(stderr,"ERROR: socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( port );
    //bind the socket and return error message if socket binding failed
    if(bind(sockfd, (struct sockaddr *)&server, sizeof(server))<0){
        fprintf(stderr,"ERROR: socket bining failed\n");
        exit(EXIT_FAILURE);
    }
    //listen for connection and return error message if any error occur
    if (listen(sockfd,1) < 0) {
        perror("ERROR: socket listening failed\n");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}
/**
*helper function to calculate the distance between two point 
*such as distance between two sensors and  between sensor and base station
**/
float Cal_distance(float x1, float y1,float x2,float y2){
return sqrt((x1-x2)^2+(y1-y2)^2);
}

int main(int argc,char* argv[]){

}