#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>

struct pipeStruct{
    int read;
    int write;
};
int main(){ 
    pipeStruct writeToUUDPClient = {};

    pipe((int*)&writeToUUDPClient);
    pid_t childPid = 0;

    childPid = fork();

    if(childPid < 0){
        printf("Process could not be created \n");
    } else if(childPid == 0){ // child process here
        close(STDOUT_FILENO);
        close(writeToUUDPClient.write);
        dup2(writeToUUDPClient.read, STDIN_FILENO);

        char *program[3] = {(char*)"./uudpchat", (char*)"client", NULL};
        execv(program[0], program);
    } else{ // the parent process
        close(writeToUUDPClient.read);
        while(true){
            time_t currentTime = time(NULL);
            char *timeString = ctime(&currentTime);
            write(writeToUUDPClient.write, timeString, strlen(timeString));
            sleep(1);
        }
        kill(childPid, SIGTERM); // should work if not use 1
        wait(NULL);
    }

    return 0;
}
