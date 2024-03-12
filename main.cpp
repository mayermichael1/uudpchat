#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h> 
#include <arpa/inet.h>

#include <errno.h>
#include <signal.h>

#include <sys/ioctl.h>
#include <termios.h>


struct vector{
    int column;
    int row;
};

static bool run = true;

static unsigned short PORT = 1212;
static const unsigned int BUFFER_LENGTH = 512;

void runClient();
void runServer();

void printUsageCode();
void clearScreen();
void setPosition(unsigned int column, unsigned int row);
vector getTerminalSize();
void setForegroundColor(unsigned int color);
void setBackgroundColor(unsigned int color);
void clearLine();
void setLineStart();

void textInput(char* buffer, const int MAX_LENGTH);

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

    termios termSettings = {};
    tcgetattr(STDIN_FILENO, &termSettings);
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
        runClient();
        printf("uudpchat client ended\n");
    }else if(strcmp(argv[1], "server") == 0){
        printf("uudpchat server started.\n");
        runServer();
        printf("uudpchat server ended\n");
    }else{
        printUsageCode();
    }

    return 0;
}

void runClient(){
    unsigned int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in addr = {}; 
    char buffer[BUFFER_LENGTH] = "hello World";

    // send to following address
    addr.sin_port = htons(PORT);
    addr.sin_family = AF_INET; 
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    while(run){
        printf("> ");
        textInput(buffer, BUFFER_LENGTH);
        sendto(socketfd, buffer, BUFFER_LENGTH, 0, (sockaddr*)&addr, sizeof(addr));

        // confirm message sent
        clearLine();
        printf("[%-7s]> %s\n","sent", buffer);
    }
}

void runServer(){
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
    while(run){
        sockaddr_in clientAddr = {};
        char ip[INET_ADDRSTRLEN] = "";
        unsigned int addrLength = sizeof(clientAddr);

        recvfrom(socketfd, buffer, BUFFER_LENGTH, 0, (sockaddr*)&clientAddr, &addrLength);

        inet_ntop(AF_INET, &clientAddr.sin_addr, ip, INET_ADDRSTRLEN);
        printf("%s:%d> %s\n",ip, ntohs(clientAddr.sin_port), buffer);

    }
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
    //TODO: remove this code when polling messages is done
    printf("\n");
    _exit(0);
}

void textInput(char* buffer, const int MAX_LENGTH){
    memset(buffer, 0, MAX_LENGTH);
    unsigned short index = 0;
    do{
        clearLine();
        printf("[%03d/%03d]> %s", index, MAX_LENGTH, buffer);
        fflush(stdout); // we need to flush the output here because we do not use '\n'

        char character;
        read(STDIN_FILENO, &character, 1);
        //TODO: read more than 1 byte if more is available

        if(character == 8 || character == 127){
            if(index > 0){ // reset last character
                index--;
                buffer[index] = 0;
            }
            continue;
        } else if(character == '\n'){
            buffer[index] = 0;
            break;
        }else if(true || (character >= 32 && character <= 126)){ // "normal" ascii character // TODO: temporary always true
            buffer[index] = character;
            index++;
        }
    }while(index<= MAX_LENGTH);

}

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
