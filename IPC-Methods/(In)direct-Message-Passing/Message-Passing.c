#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(void)
{
    //Files to store genearted producer data and consumed Consumer data
    FILE *producer_file, *consumer_file;
    pid_t pid; //used to determine process type (parent / child)

    /*Fork a child process.
    // I am assuming that the child process is the consumer
     and the parent process is the producer*/
    pid = fork();
    
}

