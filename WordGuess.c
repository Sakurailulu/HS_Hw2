//WORD GUESS GAME

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

#define MAX_CLIENT 5
#define BUFFER_SIZE 1024
#define RequestName "Welcome to Guess the Word, Please enter a name\n"

struct client{
    /**
     * this is the struct to handle the TCP client
     * in this struct it will contain the socket_fd to
     * track the fd, the buffer for send and receive
     * message, name saved the name that current user
     * enter as id
     */
    int socket_fd;
    char id[BUFFER_SIZE];

};


void initial_client(struct client* client){
    /**
     * set all the information in the client as default
     * and we can change all the private later according to
     * the need
     */

    client->socket_fd=-1;
    memset(client->id,0, sizeof(client->id));


}
/**
 * setup the tcp socket and return the sockfd
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
    if (listen(sockfd,MAX_CLIENT ) < 0) {
        perror("ERROR: socket listening failed\n");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}
/**
* function that count how many client is active
*/
int ActiveClient(const struct client* clients){
    int count=0;
    for(int i=0;i<MAX_CLIENT;i++){
        if(clients[i].socket_fd!=-1){
            count++;
        }
    }
    return count;
}
// a function to remove the client from clients
void RemoveClient(int index,struct client* clients) {
    /**
     * by given the index of clients, we can remove
     * certain client from clients by reset the client
     * into default client
     */
    initial_client(&clients[index]);
}
//find certain client by id
int FindClient(char* id,struct client* clients){
    /**
     * this function will return the socket fd of
     * certain client who has certian id
     */
    int fd=-1;

    for(int i=0;i<MAX_CLIENT;i++){
        if(clients[i].socket_fd!=-1){
            if(strcmp(id,clients[i].id)==0){
                fd=clients[i].socket_fd;
                break;
            }
        }
    }

    return fd;
}
/**
*determine the socket with the largest int value, the function should take in client 
*which is the array of client structs whose sockets we wish to check
* and return the int value of the largest socket number in the provided array of sockets
*/
int max_socket(const struct client* clients,int TCP_fd)
{
    int ans = TCP_fd;
    for (int i=0; i<MAX_CLIENT; ++i)
        if (clients[i].socket_fd > ans)
            ans = clients[i].socket_fd;
    return ans;
}

/**
 * read in the file and store in array
 * function will return the array in sorted sequence
 */
char** Read_File(char* FileName,int longest_word_length){
//read the file and store in array
char** dictionary;
FILE* fp = fopen(FileName,"r");
int count=0;
char wordBuff[BUFFER_SIZE];
while(fscanf(fp, "%s", wordBuff) != EOF){
    if(strlen(wordBuff)<longest_word_length){
            count++;
        dictionary = realloc(dictionary, (count)*sizeof(*dictionary));
        dictionary[count-1] = malloc(strlen(wordBuff)+1);
        strcpy(dictionary[count-1], wordBuff);
    }
}

return dictionary;
}

/**
 * select a word randomly from dictionary and referred as secret word
*/
char* GetSecretWord(char** dictionary, int seed){
srand(seed);
char* SecretWord=dictionary[rand()%sizeof(dictionary)];
return SecretWord;
}

/**
 * void function that setup the game
 * in this function secret word will be randomly selected and printed out
 * all client will be initialized
 */
 void GameSetUp(char* FileName,int longest_word_length ,int seed,char* SecretWord,struct client* clients){

     char** dictionary=Read_File(FileName,longest_word_length);
     SecretWord=GetSecretWord(dictionary,seed);
     /* initial all the client  */
     for (int i = 0; i < MAX_CLIENT; i++) {
        initial_client(&clients[i]);
     }
     printf("Secret word is: %s\n",SecretWord);

 }

/**
 *initialize an fd set with a list of ports, as well as stdin
 * the function will take in two parameter:
 * clients is the array of client structs whose ports we wish to listen to in the fd set set
 * TCP_fd is  the fd set to initialize
*/
fd_set selectOnSockets(const struct client* clients, int TCP_fd)
{
    fd_set set;
    FD_ZERO(&set);
    FD_SET(TCP_fd, &set);
    for (int i=0; i<MAX_CLIENT; ++i) {
        if (clients[i].socket_fd != -1) {
            FD_SET(clients[i].socket_fd, &set);
        }
    }
    select(max_socket(clients,TCP_fd)+1, &set, NULL, NULL, NULL);
    return set;
}

/**
 * function that handle name assigning including check if the name is occupied
 */
void ChangingName(char* name,struct client* clients,struct client* sender,char* secretWord){
    //if the name is already taken
    if(FindClient(name,clients)!=-1){
        char message[1024];
        int messageLength = snprintf
                (
                        message,
                        1024,
                        "Username %s is already taken, please enter a different username\n",
                        name
                );
        send(sender->socket_fd, message, messageLength, 0);
    }
    //set up the name for client
    else{
        strcpy(sender->id, name);
        char message[1024];
        int messageLength = snprintf
                (
                        message,
                        1024,
                        "There are %d player(s) playing. The secret word is %ld letter(s)\n",
                        ActiveClient(clients),
                        sizeof(secretWord)
                );
        
        send(sender->socket_fd,message, messageLength,0);
    }


}

/**
*function that count how many correct letter is in the guess word compare to the secret word
*/
int CorrectLetter(const char* guess, const char* SecretWord){

    int size = strlen(SecretWord);
    int count = 0;
    char copy[size];
    strcpy(copy,SecretWord);

    char guessCopy[size];
    strcpy(guessCopy,guess);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (copy[j] == guessCopy[i] && guessCopy[i] != '\0') {
                copy[j] = '\0';
                guessCopy[j] = '\0';
                count++;
            }
        }
    }
    return count;
}




/**
*function that count how many correct placed letter is in the current guess word compare to the secret word
*/
int CorrectPlaced(const char* guess,const char* SecretWord){
    int count=0;
    for(int i=0;i<strlen(guess);i++){
        if(SecretWord[i]==guess[i]){
            count++;
        }
    }
    return count;
}

int main(int argc, char* argv[]){
    int  Newsockfd,port;
    struct client clients[MAX_CLIENT];
    //if not enough argument has been given
    if(argc<5){
        fprintf(stderr,"ERROR: the correct usage is './word_guess.out [seed] [port] [dictionary_file][longest_word_length]'\n");
        exit(1);
    }
    port = atoi(argv[2]);
    int TCP_fd = Set_TCP_Socket(port);
    char* SecretWord;
    GameSetUp(argv[3],atoi(argv[4]),atoi(argv[1]),SecretWord,clients);
    while(true){
        fd_set fdset = selectOnSockets(clients, TCP_fd);
        if (FD_ISSET(TCP_fd,&fdset)){
            struct sockaddr_in onelayer;
            onelayer.sin_family = PF_INET;
            bzero(&onelayer, sizeof(onelayer));
            unsigned int onelayersize = sizeof(onelayer);
            for(int i=0;i<MAX_CLIENT;i++){
                if(clients[i].socket_fd<0){
                    clients[i].socket_fd=accept(TCP_fd, (struct sockaddr *) &onelayer,&onelayersize);
                    if(clients[i].socket_fd<0){
                        fprintf(stderr,"ERROR: Accept failed\n");
                    }
                    clients[i].id[0] = '\0';
                    send(clients[i].socket_fd, RequestName, strlen(RequestName), 0);
                    break;
                }
            }
        }
        for(int i=0;i<MAX_CLIENT;i++){
            if(clients[i].socket_fd!=-1&&FD_ISSET(clients[i].socket_fd, &fdset)){
                char buffer[BUFFER_SIZE];
                ssize_t countRead=read(clients[i].socket_fd,buffer,BUFFER_SIZE-1);
                buffer[countRead]='\0';
                if(countRead==0){
                    printf("%s (socket %d) has disconnected\n", clients[i].id, clients[i].socket_fd);
                    close(clients[i].socket_fd);
                    clients[i].socket_fd = -1;
                }
                //if current client do not have entered id yet, therefore the bytes read here is the name for this client
                if(clients[i].id[0]=='\0'){
                    ChangingName(buffer,clients,&clients[i],SecretWord);
                }
                //otherwise the buffer is the guess that received from client
                else{
                    //if there is any client send invalid guess due to the invalid length
                    //send the error message only to the client with invalid length
                    if(strlen(buffer)!=strlen(SecretWord)){
                        char message[1024];
                        int messageLength = snprintf
                        (
                         message,
                         1024,
                            "Invalid guess length. The secret word is %ld letter(s)\n",
                            strlen(SecretWord)
                         );

                        send(clients[i].socket_fd,message, messageLength,0);
                    }
                    //if the client guess the word correctly
                    else if(strcmp(buffer,SecretWord)==0){
                        //broadcast the message that one player has guessed the word
                        char* message=malloc(BUFFER_SIZE* sizeof(char));
                        strcat(message, clients[i].id);
                        strcat(message,"has correctly the word ");
                        strcat(message, SecretWord);
                        strcat(message, "\n");
                        for(int index=0;index<MAX_CLIENT;index++){
                            send(clients[index].socket_fd,message, strlen(message),0);
                            RemoveClient(index,clients);
                        }
                        //disconnect all the players and restart the game
                        GameSetUp(argv[3],atoi(argv[4]),atoi(argv[1]),SecretWord,clients);
                    }
                    //the guess word is valid length, but the word itself is not correct
                    else{
                        char message[1024];
                        int messageLength = snprintf
                        (
                            message,
                            1024,
                            "%s guessed %s : %d letter(s) were correct and %d letters(s) were correctly placed\n",
                            clients[i].id,
                            buffer,
                             CorrectLetter(buffer,SecretWord),
                             CorrectPlaced(buffer,SecretWord)
                        );
            
                        for(int i=0;i<MAX_CLIENT;i++){
                            send(clients[i].socket_fd,message,messageLength,0);
                        }
                    }

                }
            }
        }

    }



}