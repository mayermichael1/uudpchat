#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>

//#define CUR_STYLE
//#define CAMEL_CASE
//#define SNAKE_CASE
#define PASCAL_CASE
///==========================================================================///
///     current style                                                        ///
///==========================================================================///
#ifdef CUR_STYLE

struct pipeStruct{
    int read;
    int write;
};

void childProcess(pipeStruct writeToUUDPClient){
    close(STDOUT_FILENO);
    close(writeToUUDPClient.write);
    dup2(writeToUUDPClient.read, STDIN_FILENO);

    char *program[3] = {(char*)"./uudpchat", (char*)"client", NULL};
    execv(program[0], program);
}

void parentProcess(pipeStruct writeToUUDPClient, pid_t childPid){
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

int main(){ 
    pipeStruct writeToUUDPClient = {};

    pipe((int*)&writeToUUDPClient);
    pid_t childPid = 0;

    childPid = fork();

    if(childPid < 0){
        printf("Process could not be created \n");
    } else if(childPid == 0){ // child process here
        childProcess(writeToUUDPClient);
    } else{ // the parent process
        parentProcess(writeToUUDPClient, childPid);
    }

    return 0;
}

#endif // CURSTYLE

///==========================================================================///
///     everything camel case                                                ///
///==========================================================================///

#ifdef CAMEL_CASE
struct pipeStruct
{
    int read;
    int write;
};

void 
childProcess (pipeStruct writeToUUDPClient )
{
    close(STDOUT_FILENO);
    close(writeToUUDPClient.write);
    dup2(writeToUUDPClient.read, STDIN_FILENO);

    char *program[3] = {(char*)"./uudpchat", (char*)"client", NULL};
    execv(program[0], program);
}

void 
parentProcess (pipeStruct writeToUUDPClient, pid_t childPid)
{
    close(writeToUUDPClient.read);
    while(true)
        {
            time_t currentTime = time(NULL);
            char *timeString = ctime(&currentTime);
            write(writeToUUDPClient.write, timeString, strlen(timeString));
            sleep(1);
        }

    kill(childPid, SIGTERM); // should work if not use 1
    wait(NULL);
}

int 
main ()
{
    pipeStruct writeToUUDPClient = {};

    pipe((int*)&writeToUUDPClient);
    pid_t childPid = 0;

    childPid = fork();

    if(childPid < 0)
        {
            printf("Process could not be created \n");
        } 
    else if(childPid == 0) // child process here
        { 
            childProcess(writeToUUDPClient);
        } 
    else // the parent process
        { 
            parentProcess(writeToUUDPClient, childPid);
        }

    return 0;
}

#endif // CAMEL_CASE



///==========================================================================///
///     everything snake case                                                ///
///==========================================================================///

#ifdef SNAKE_CASE
struct pipe_struct
{
  int read;
  int write;
};

void 
child_process (pipe_struct write_to_uudp_client )
{
  close(STDOUT_FILENO);
  close(write_to_uudp_client.write);
  dup2(write_to_uudp_client.read, STDIN_FILENO);

  char *program[3] = {(char*)"./uudpchat", (char*)"client", NULL};
  execv(program[0], program);
}

void 
parent_process (pipe_struct write_to_uudp_client, pid_t child_pid)
{
  close(write_to_uudp_client.read);
  while(true)
    {
      time_t current_time = time(NULL);
      char *time_string = ctime(&current_time);
      write(write_to_uudp_client.write, time_string, strlen(time_string));
      sleep(1);
    }

  kill(child_pid, SIGTERM); // should work if not use 1
  wait(NULL);
}

int 
main ()
{
  pipe_struct write_to_uudp_client = {};

  pipe((int*)&write_to_uudp_client);
  pid_t child_pid = 0;

  child_pid = fork();

  if(child_pid < 0)
    {
      printf("Process could not be created \n");
    } 
  else if(child_pid == 0) // child process here
    { 
      child_process(write_to_uudp_client);
    } 
  else // the parent process
    { 
      parent_process(write_to_uudp_client, child_pid);
    }

  return 0;
}

#endif // SNAKE_CASE

///==========================================================================///
///     PASCAL CASE                                                          ///
///==========================================================================///

#ifdef PASCAL_CASE
struct PipeStruct
{
    int Read;
    int Write;
};

void 
RunChild (PipeStruct WriteToUUDPClient )
{
    close(STDOUT_FILENO);
    close(WriteToUUDPClient.Write);
    dup2(WriteToUUDPClient.Read, STDIN_FILENO);

    char *program[3] = {(char*)"./uudpchat", (char*)"client", NULL};
    execv(program[0], program);
}

void 
RunParent (PipeStruct WriteToUUDPClient, pid_t ChildPid)
{
    close(WriteToUUDPClient.Read);
    while(true)
        {
            time_t CurrentTime = time(NULL);
            char *TimeString = ctime(&CurrentTime);
            write(WriteToUUDPClient.Write, TimeString, strlen(TimeString));
            sleep(1);
        }

    kill(ChildPid, SIGTERM); // should work if not use 1
    wait(NULL);
}

int 
main ()
{
    PipeStruct WriteToUUDPClient = {};

    pipe((int*)&WriteToUUDPClient);
    pid_t ChildPid = 0;

    ChildPid = fork();

    if(ChildPid < 0)
        {
            printf("Process could not be created \n");
        } 
    else if(ChildPid == 0) // child process here
        { 
            RunChild(WriteToUUDPClient);
        } 
    else // the parent process
        { 
            RunParent(WriteToUUDPClient, ChildPid);
        }

    return 0;
}

#endif // PASCAL_CASE
