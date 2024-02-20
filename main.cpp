#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>

#include <errno.h>

void runClient();
void runServer();
void printUsageCode();

int main(int argc, char **argv){
    if(argc != 2) {
        printUsageCode();
        return 0;
    }

    if(strcmp(argv[1],"client") == 0){
        printf("uudpchat client started.\n");
        runClient();
    }else if(strcmp(argv[1], "server") == 0){
        printf("uudpchat server started.\n");
        runServer();
    }else{
        printUsageCode();
    }
    return 0;
}

void runClient(){
    unsigned int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in addr = {}; 
    char buffer[256] = "hello World";

    addr.sin_port = htons(1212);
    addr.sin_family = AF_INET; 
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    while(1){
        fgets(buffer, 255, stdin);

        unsigned int length = strlen(buffer);
        if(buffer[length-1] == '\n') buffer[length-1] = 0;

        sendto(socketfd, buffer, 256, 0, (sockaddr*)&addr, sizeof(addr));
    }
}

void runServer(){
    unsigned int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in addr = {}; 

    addr.sin_port = htons(1212);
    addr.sin_family = AF_INET; 
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int error = bind(socketfd, (sockaddr*)&addr, sizeof(addr));

    if(error == -1){
        printf("could not bind. Error: %d\n", errno);
        return;
    }

    char buffer[256] = "";
    while(1){
        recvfrom(socketfd, buffer, 256, 0, 0, 0);
        printf("> %s\n", buffer);
    }
}

void printUsageCode(){
    printf("Wrong usage.\n");
    printf("\n");
    printf("Usage:\n");
    printf("uudpchat [server | client]\n");
}
