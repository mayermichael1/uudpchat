#include "../include/tui.h"

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

void clearScreen(){
    printf("\e[2J");
}

void setPosition(unsigned int column, unsigned int row){
    printf("\e[%d;%dH", column, row);
}

vector getTerminalSize(){
    vector winSize;
    winSize.row = 0;
    winSize.column = 0;

    winsize terminalSize;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminalSize);
    winSize.column = terminalSize.ws_col;
    winSize.row = terminalSize.ws_row;

    return winSize;
}

void setForegroundColor(unsigned int color){
    if(color > 7) color = 7;
    printf("\e[%dm",(30+color));
}

void setBackgroundColor(unsigned int color){
    if(color > 7) color = 7;
    printf("\e[%dm",(40+color));
}


void clearLine(){
    setLineStart();
    printf("\e[K");
}
void setLineStart(){
    printf("\e[1G");
}
