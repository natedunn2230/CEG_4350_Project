#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h> 
#include <sys/ipc.h> 
#include <sys/msg.h>
#include <time.h> 

#define DATA_SIZE 100

// structure for the message
struct buffer { 
    long mesg_type; 
    int payload[1]; 
} msg; 

int main() 
{ 
    FILE * producer_file; // file to write data to
    key_t key; // key for accessing queue
    int queue; // queue identifier
    int new_data; // data being generated

    // set seed for random generator
    srand(time(NULL));

    // generate a key for the queue
    key = ftok("my_queue", 2230); 

    // create message queue indentifier
    // or get existing one
    queue = msgget(key, 0666 | IPC_CREAT); 

    //producer file for checking cooperation between processes
    producer_file = fopen("/tmp/MessageProducerOutput.txt", "w+");

    for(int i = 0; i <DATA_SIZE; i++)
    {
        // generate data and set buffer lement
        new_data = rand() % 10000;
        msg.mesg_type = 1; 
        msg.payload[0] = new_data;

        // send generated integer data to queue
        msgsnd(queue, &msg, sizeof(msg), 0); 

        // write data to file
        fprintf(producer_file, "%d\n", new_data); // write to output file
    }

    // close data file
    fclose(producer_file); 

    return 0; 
} 