#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <semaphore.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include <sys/shm.h>
#include <time.h>

#define BUFFER_SIZE 10
#define TOTAL_DATA 100
#define IN_INDEX BUFFER_SIZE
#define OUT_INDEX BUFFER_SIZE + 1

int main()
{
    // buffer variables (1-100 bytes -> buffer, 101 -> in value, 102 -> out value)
    // transfered between processes using shared memory
    int * shared_mem;

    // keys for shared memory of variables
    key_t shared_mem_key = ftok("buffer", 1);

    // get id for shared memory
    int shared_memory_size = (BUFFER_SIZE + 2) * sizeof(int);
    int shared_mem_id = shmget(shared_mem_key, shared_memory_size, 0666 | IPC_CREAT);

    // assign variables to shared memory
    shared_mem = (int *)shmat(shared_mem_id, NULL,0);

    // check to see if shared memory could be allocated
    if (shared_mem == (void *) -1)
    {
        perror("shmat failed");
        exit(1);
    }
    
    shared_mem[IN_INDEX] = 0; // set in
    shared_mem[OUT_INDEX] = 0; // set out

    // semaphores for process coordination
    sem_t * mutex;
    sem_t * full;
    sem_t * empty;

    pid_t pid;

    // create semaphores
    mutex = sem_open("mutex", O_CREAT, 0644, 1);
    full = sem_open("full", O_CREAT, 0644, 0);
    empty = sem_open("empty", O_CREAT, 0644, BUFFER_SIZE);

    // check to see if semaphores were created successfully
    if (mutex == SEM_FAILED || full == SEM_FAILED || empty == SEM_FAILED)
    {
        perror("semphore creation failed");
        exit(1);
    }

    /*Fork a child process.
    // I am assuming that the child process is the consumer
    and the parent process is the producer*/
    pid = fork();

    // error during proces creation
    if (pid == -1)
        exit(1);
    else if(pid == 0)
    {
        // child process takes role of the consumer

        // consumer file for checking cooperation between processes  
        FILE * consumer_file = fopen("/tmp/SharedConsumerOutput.txt", "w+");

        for(int i = 0; i < TOTAL_DATA; i++)
        {
            int consumed_data;

            // wait for critical section to be available 
            sem_wait(full);
            sem_wait(mutex);

            // consume data item from the buffer
            consumed_data = shared_mem[shared_mem[OUT_INDEX]];
            shared_mem[OUT_INDEX] = (shared_mem[OUT_INDEX] + 1) % BUFFER_SIZE;

            fprintf(consumer_file, "%d\n", consumed_data); // write to output file

            // release the lock
            sem_post(mutex);
            sem_post(empty);
        }

        fclose(consumer_file);
    }
    else
    {
        // parent process takes role of the producer

        // producer file for checking cooperation between processes  
        FILE * producer_file = fopen("/tmp/SharedProducerOutput.txt", "w+");
        srand(time(NULL)); // seed for number generator

        for(int i = 0; i < TOTAL_DATA; i++)
        {   
            int new_data = rand() % 100000;

            // wait for critical section to become available
            sem_wait(empty);
            sem_wait(mutex);

            // add item to buffer
            shared_mem[shared_mem[IN_INDEX]] = new_data;
            shared_mem[IN_INDEX] = (shared_mem[IN_INDEX] + 1) % BUFFER_SIZE;

            fprintf(producer_file, "%d\n", new_data); // write to output file

            // release the lock
            sem_post(mutex);
            sem_post(full);
        }

        fclose(producer_file);
    }

    // remove attachment of shared memory
    shmdt(shared_mem);

    return 0;
}