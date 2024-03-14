#include <stdio.h>
#include <string.h>

#include <signal.h>

#include <termios.h>

#include "include/tui.h"
#include "include/networking.h"
#include "include/client.h"
#include "include/server.h"

static bool run = true;

void printUsageCode();

void handleCtrlC(int signal);

int main(int argc, char **argv){
    if(argc != 2) {
        printUsageCode();
        return 0;
    }

    vector winSize = getTerminalSize();
    clearScreen();
    setPosition(1,1);
    printf("Initialized Screen with size: %d * %d\n", winSize.column, winSize.row);

    // not needed apparently 
    //setvbuf(stdin, NULL, _IONBF, 0); // disable input buffer

    termios termSettings, orignalTermSettings = {};
    tcgetattr(STDIN_FILENO, &orignalTermSettings);
    termSettings = orignalTermSettings; // set new Terminal settings
    termSettings.c_lflag &= ~(ICANON | ECHO); // disable ICANON mode and ECHO
    // TODO: implement something to disable arrow keys up and down
    termSettings.c_cc[VMIN] = 1; // return from read calls after 1 byte is read
    tcsetattr(STDIN_FILENO, TCSANOW, &termSettings);

    // handle ctrl c interrupt
    struct sigaction action;
    action.sa_handler = handleCtrlC;
    sigaction(SIGINT, &action, NULL);

    // start application in server or client mode
    if(strcmp(argv[1],"client") == 0){
        printf("uudpchat client started.\n");
        runClient(&run);
        printf("uudpchat client ended\n");
    }else if(strcmp(argv[1], "server") == 0){
        printf("uudpchat server started.\n");
        runServer(&run);
        printf("uudpchat server ended\n");
    }else{
        printUsageCode();
    }

    // reset termianl to canon mode
    tcsetattr(STDIN_FILENO, TCSANOW, &orignalTermSettings);

    return 0;
}



void printUsageCode(){
    printf("Wrong usage.\n");
    printf("\n");
    printf("Usage:\n");
    printf("uudpchat [server | client]\n");
}

void handleCtrlC(int signal){
    (void)signal;
    run = false;
}

