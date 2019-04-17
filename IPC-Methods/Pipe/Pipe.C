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

int main(void)
{   
    //Files to store genearted producer data and consumed Consumer data
    FILE *producer_file, *consumer_file;

    int fd[2]; //pipe accessor
    pid_t pid; //used to determine process type (parent / child)

    //Pipe creation
    if(pipe(fd) < 0)
        exit(1);

    /*Fork a child process.
    I am assuming that the child process is the consumer
    and the parent process is the producer*/
    pid = fork();

    if (pid == -1)
        exit(1);
    else if(pid == 0)
    {
        // child process takes role of the consumer
        
        // # of bytes returned from read() and data read from pipe
        int read_size, incoming_data;

        // consumer file for checking cooperation between processes
        consumer_file = fopen("/tmp/PipeConsumerOutput.txt", "w+");

        // closes unused end of the pipe
        close(fd[WRITE_END]);

        // read each integer from the pipe and write it to output file
        while(1)
        {
            // read from pipe
            read_size = read(fd[READ_END], &incoming_data, BUFFER_SIZE); 

            // we are done reading if there is nothing
            if(read_size <= 0) break;

            // write data to file
            fprintf(consumer_file, "%d\n", incoming_data);
        }

        // close read end of the pipe and the outpuf file
        close(fd[READ_END]);
        fclose(consumer_file);
    }
    else 
    {
        // parent process takes role of the producer
        
        // set seed for random generator
        srand(time(0));

        //producer file for checking cooperation between processes
        producer_file = fopen("/tmp/PipeProducerOutput.txt", "w+");

        // close unused side of pipe
        close(fd[READ_END]);

        // create 100 random integers
        for(int i = 0; i < DATA_SIZE; i++)
        {
            // limited random value range to 10000 to improve readibility
            int new_data = rand() % 10000;

            write(fd[WRITE_END], &new_data, sizeof(new_data)); // write to pipe

            fprintf(producer_file, "%d\n", new_data); // write to output file
        }
        
        // close write end of the pipe and the output file
        close(fd[WRITE_END]); 
        fclose(producer_file); 
    }

    return 0;
}