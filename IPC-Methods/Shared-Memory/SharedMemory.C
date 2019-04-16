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

#define BUFFER_SIZE 2
#define TOTAL_DATA 10
#define IN_INDEX BUFFER_SIZE
#define OUT_INDEX BUFFER_SIZE + 1

int main()
{
    // buffer variables
    int * shared_mem;

    // keys for shared memory of variables
    key_t buffer_key = ftok("buffer", 1);
    key_t in_out_key = ftok("in_out", 2);

    // get id for shared memory
    int shared_mem_id = shmget(buffer_key, OUT_INDEX * sizeof(int), 0666 |IPC_CREAT);

    // assign variables to shared memory
    shared_mem = (int *)shmat(shared_mem_id, NULL,0);
    
    shared_mem[IN_INDEX] = 0; // set in
    shared_mem[OUT_INDEX] = 0; // set out

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

        int consumed_data;
        printf("%d\n", 1);
        sem_wait(full);
        printf("%d\n", 2);

        sem_wait(mutex);

        consumed_data = shared_mem[shared_mem[OUT_INDEX]];
        shared_mem[OUT_INDEX] = (shared_mem[OUT_INDEX] + 1) % BUFFER_SIZE;

        printf("CONSUMED %d\n", consumed_data);

        sem_post(mutex);
        sem_post(empty);
        printf("%d\n", shared_mem[IN_INDEX]);
        printf("%d\n", shared_mem[OUT_INDEX]);

    }
    else
    {
        // parent process takes role of the producer
        int new_data = rand() % 100000;

        printf("%d\n", 3);

        sem_wait(empty);

        printf("%d\n", 4);

        sem_wait(mutex);

        // add item to buffer
        shared_mem[shared_mem[IN_INDEX]] = new_data;
        shared_mem[IN_INDEX] = (shared_mem[IN_INDEX] + 1) % BUFFER_SIZE;

        printf("PRODUCED %d\n", new_data);

        sem_post(mutex);
        sem_post(full);

        printf("p %d\n",  shared_mem[IN_INDEX]);
    }


    return 0;
}