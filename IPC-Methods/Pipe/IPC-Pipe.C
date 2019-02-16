#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wait.h>

#define DATA_SIZE 100
#define BUFFER_SIZE sizeof(int)
#define READ_END 0
#define WRITE_END 1

void ProducerJob(int *, FILE*);
void ConsumerJob(int *, FILE*);


int main(void)
{   
    //Files to store genearted producer data and consumed Consumer data
    FILE *producer_file, *consumer_file;
    int fd[2]; //pipe accessor
    pid_t pid; //used to determine process type (parent / child)

    //Pipe creation
    if(pipe(fd) == -1)
    {
        fprintf(stderr, "Pipe creation unsuccessful");
    }

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
        ProducerJob(fd, producer_file);
    }
    else 
    {
        // child process takes role of the consumer
        ConsumerJob(fd, consumer_file);
    }

    return 0;
}

/*
Produces 100 random integer values, and passes them indiviually through
the writed end of the pipe for the consumer to read later
*/
void ProducerJob(int *pipe_accessor, FILE *producer_file)
{
    // set seed for random generator
    srand(time(NULL));

    //producer file for checking cooperation between processes
    producer_file = fopen("/tmp/ProducerOutput.txt", "w+");

    // close unused side of pipe
    close(pipe_accessor[READ_END]);

    // create 100 random integers
    for(int i = 0; i < DATA_SIZE; i++)
    {
        // limited random value range to 1000 to improve readibility
        int new_data = rand() % 1000;
        write(pipe_accessor[WRITE_END], &new_data, sizeof(new_data)); // write to pipe
        fprintf(producer_file, "%d\n", new_data); // write to output file
    }
    
    // close write end of the pipe and the output file
    close(pipe_accessor[WRITE_END]); 
    fclose(producer_file); 
}

/*
Reads 100 random integers from the pipe that were produced
by the producer process
*/
void ConsumerJob(int *pipe_accessor, FILE *consumer_file)
{
    // consumer file for checking cooperation between processes
    consumer_file = fopen("/tmp/ConsumerOutput.txt", "w+");

    // closes unused end of the pipe
    close(pipe_accessor[WRITE_END]);

    // read each integer from the pipe and write it to output file
    for(int i = 0; i < DATA_SIZE; i++){
        int incoming_data;

        read(pipe_accessor[READ_END], &incoming_data, BUFFER_SIZE);
        fprintf(consumer_file, "%d\n", incoming_data);
    }

    // close read end of the pipe and the outpuf file
    close(pipe_accessor[READ_END]);
    fclose(consumer_file);

}