#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>

#include <errno.h>

int main(int argc, char **argv){
    if(argc != 2) return 1;

    unsigned int socketfd = socket(AF_INET, SOCK_DGRAM, 0);

    if(strcmp(argv[1],"client") == 0){
        printf("uudpchat client started.\n");
        char buffer[256] = "hello World";

        sockaddr_in addr = {}; 
        addr.sin_port = htons(1212);
        addr.sin_family = AF_INET; 
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);


        sendto(socketfd, buffer, 256, 0, (sockaddr*)&addr, sizeof(addr));
        printf("ERROR: %d\n", errno);

    }else{
        printf("uudpchat server started.\n");

        sockaddr_in addr = {}; 
        addr.sin_port = htons(1212);
        addr.sin_family = AF_INET; 
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
        int error = bind(socketfd, (sockaddr*)&addr, sizeof(addr));

        if(error == -1){
            printf("could not bind. Error: %d\n", errno);
            return 1;
        }

        char buffer[256] = "";
        recvfrom(socketfd, buffer, 256, 0, 0, 0);
        printf("Received: %s\n", buffer);
    }
    return 0;
}
