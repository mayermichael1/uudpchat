#include "../include/client.h"

#include "../include/networking.h"
#include "../include/tui.h"

#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>

#include <errno.h>

#include <poll.h>
#include <unistd.h>

#include <string.h>

void runClient(bool *run){
    unsigned int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in addr = {}; 
    char buffer[BUFFER_LENGTH] = {};
    unsigned int bufferCurrentIndex = 0;

    // send to following address
    addr.sin_port = htons(PORT);
    addr.sin_family = AF_INET; 
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    // initialize the terminal
    clearLine();
    printf("[%03d/%03d]> %s", bufferCurrentIndex, (BUFFER_LENGTH-1), buffer);
    fflush(stdout);

    // prepare stdin poll
    pollfd polls[2] = {};
    polls[0].fd = STDIN_FILENO;
    polls[0].events = POLLIN; 

    polls[1].fd = socketfd;
    polls[1].events = POLLIN; 

    while(*run){
         
        int ready = poll(polls, 2, -1);

        if(ready <= 0){
            continue;
        }
        if(polls[0].revents & POLLIN){ // check STDIN_FILENO for poll
            char character;
            read(polls[0].fd, &character, 1); 

            if(bufferCurrentIndex > 0 && (character == 8 || character == 127)){
                bufferCurrentIndex--;
                buffer[bufferCurrentIndex] = 0;
            } else if(character == '\n'){
                setLineStart();
                printf("[%-8s]>\n", "You:");
                sendto(socketfd, buffer, BUFFER_LENGTH, 0, (sockaddr*)&addr, sizeof(addr));
                memset(buffer, 0, BUFFER_LENGTH);
                bufferCurrentIndex = 0;
            } else if((character >= 32 && character <= 126) && bufferCurrentIndex < (BUFFER_LENGTH - 1)){ // "normal" ascii character // TODO: temporary always true
                buffer[bufferCurrentIndex] = character;
                bufferCurrentIndex++;
            }
            clearLine();
            printf("[%03d/%03d]> %s", bufferCurrentIndex, (BUFFER_LENGTH-1), buffer);
            fflush(stdout);
        }
        if(polls[1].revents & POLLIN){
            char recBuffer[BUFFER_LENGTH] = "";
            recv(socketfd, recBuffer, BUFFER_LENGTH, 0);
            setLineStart();
            printf("[%-8s]> %s\n", "SRV", recBuffer);
            clearLine();
            printf("[%03d/%03d]> %s", bufferCurrentIndex, (BUFFER_LENGTH-1), buffer);
            fflush(stdout);
        }

    }
}
