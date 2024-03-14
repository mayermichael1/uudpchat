#include "../include/server.h"
#include "../include/settings.h"

#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>

#include <errno.h>

#include <poll.h>
#include <string.h>

void runServer(bool *run){
    unsigned int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in addr = {}; 

    addr.sin_port = htons(PORT);
    addr.sin_family = AF_INET; 
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int error = bind(socketfd, (sockaddr*)&addr, sizeof(addr));

    if(error == -1){
        printf("could not bind. Error: %d\n", errno);
        return;
    }

    char buffer[BUFFER_LENGTH] = "";

    sockaddr_in clientAddr = {};
    char ip[INET_ADDRSTRLEN] = "";
    unsigned int addrLength = sizeof(clientAddr);

    pollfd polls[1] = {};
    polls[0].fd = socketfd;
    polls[0].events = POLLIN;

    while(*run){

        int ready = poll(polls, 1, -1);
        if( ready <= 0){
            continue;
        }
        if(polls[0].revents & POLLIN){
            recvfrom(socketfd, buffer, BUFFER_LENGTH, 0, (sockaddr*)&clientAddr, &addrLength);
            inet_ntop(AF_INET, &clientAddr.sin_addr, ip, INET_ADDRSTRLEN);
            printf("%s:%d> %s\n",ip, ntohs(clientAddr.sin_port), buffer);

            strcpy(buffer,"Message sent");
            sendto(socketfd, buffer, strlen(buffer), 0, (sockaddr*)&clientAddr, addrLength);

        }

    }
}
