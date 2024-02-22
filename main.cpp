#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <errno.h>

#include <signal.h>

static bool run = true;

void runClient();
void runServer();
void printUsageCode();

void handleInt(int signal);

int main(int argc, char **argv){
    if(argc != 2) {
        printUsageCode();
        return 0;
    }

    struct sigaction action;
    action.sa_handler = handleInt;
    sigaction(SIGINT, &action, NULL);

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

    while(run){
        printf("> ");
        fgets(buffer, 255, stdin);

        unsigned int length = strlen(buffer);
        if(buffer[length-1] == '\n') buffer[length-1] = 0;

        sendto(socketfd, buffer, 256, 0, (sockaddr*)&addr, sizeof(addr));
    }

    printf("uudpchat client ended\n");

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
    while(run){
        sockaddr_in clientAddr = {};
        char ip[INET_ADDRSTRLEN] = "";
        unsigned int addrLength = sizeof(clientAddr);

        recvfrom(socketfd, buffer, 256, 0, (sockaddr*)&clientAddr, &addrLength);
        inet_ntop(AF_INET, &clientAddr.sin_addr, ip, INET_ADDRSTRLEN);
        printf("%s:%d> %s\n",ip, ntohs(clientAddr.sin_port), buffer);

    }

    printf("uudpchat server ended\n");
}

void printUsageCode(){
    printf("Wrong usage.\n");
    printf("\n");
    printf("Usage:\n");
    printf("uudpchat [server | client]\n");
}

void handleInt(int signal){
    (void)signal;
    run = false;
}
