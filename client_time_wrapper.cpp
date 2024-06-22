#include <stdio.h>
#include <time.h>
#include <unistd.h>

struct pipeStruct{
    int write;
    int read;
};
int main(){ 
    pipeStruct writeToUUDPClient = {};

    pipe((int*)&writeToUUDPClient);
    pid_t childPid = 0;

    childPid = fork();
    if(childPid < 0){
        printf("Process could not be created \n");
    }else if(childPid == 0){

    }else{

    }

    while(1){
        time_t currentTime = time(NULL);
        printf("%s", ctime(&currentTime));
        sleep(1);
    }
    return 0;
}
