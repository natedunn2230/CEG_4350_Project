#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

void ProducerProcess(FILE *, int, int flag[]);

int main()
{
    //Files to store genearted producer data and consumed Consumer data
    FILE *producer_file, *consumer_file;
    pid_t pid;

    // petersons solution used for conditional checking
    int turn; // 0 - producer, 1 - consumer
    int flag[2]; // 0 - false, 1 - true
    int i = 0; // producer
    int j = 1; // consumer

    /*Fork a child process.
    // I am assuming that the child process is the consumer
    and the parent process is the producer*/
    pid = fork();

    if (pid == -1)
        exit(1);

    else if(pid > 0)
    {
        // parent process takes role of the producer
        while(1)
        {
            flag[i] = 1;
            turn = j;

            while(flag[j] && turn == j) // wait for consumer to finish
            
            // CRITICAL SECTION

            flag[i] = 0; // release hold
        }
        
        wait(NULL);
    }
    else 
    {
        // child process takes role of the consumer
        while(1)
        {
            flag[j] = 1;
            turn = i;

            while(flag[i] && turn == i) // wait for producer to finish

            // CRITICAL SECTION 

            flag[j] = 0; // release hold
        }
    }

    return 0;
}