#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define DATA_SIZE 100

void ProducerJob(FILE *);
void ConsumerJob(FILE *);

int main(void)
{
     //Files to store genearted producer data and consumed Consumer data
    FILE *producer_file, *consumer_file;
    pid_t pid; //used to determine process type (parent / child)
    /*Fork a child process.
    // I am assuming that the child process is the consumer
    and the parent process is the producer*/
    pid = fork();

    if (pid == -1)
    {
        fprintf(stderr, "Process creation unsuccessful");
    }
    else if(pid > 0)
    {
        // parent process takes role of the producer
        ProducerJob(producer_file);
    }
    else 
    {
        // child process takes role of the consumer
        ConsumerJob(consumer_file);
    }

    return 0;
}

/*
Produces 100 random integer values, and passes them indiviually to
the message queue for the consumer to read from
*/
void ProducerJob(FILE* producer_file)
{
    // create message queue (if non-existent) and

    // set seed for random generator
    srand(time(NULL));

    for(int i = 0; i < DATA_SIZE; i++)
    {   
        // limit integer value to 10000 to improve readability
        int new_data = rand() % 10000;
    }
}

/*
Reads 100 random integers from the message queue that were produced
by the producer process
*/
void ConsumerJob(FILE* consumer_file)
{
    printf("started reading\n");
}

