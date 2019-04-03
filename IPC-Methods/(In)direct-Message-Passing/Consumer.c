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
    FILE * consumer_file; // file to write data to
    key_t key; // key for accessing queue
    int queue; // queue identifier
    int amount; // amount of data within queue
    int count; // amount of data recieved

    // buffer for checking size of queue periodically
    struct msqid_ds buf;

    // generate key
    key = ftok("my_queue", 2230); 

    // obtain identifier
    queue = msgget(key, 0666 | IPC_CREAT); 

    count = 0;

    //consumer file for checking cooperation between processes
    consumer_file = fopen("/tmp/MessageConsumerOutput.txt", "w+");

    // read from the queue
    while(1){
        
        // read how many elements are in the queue
        int reader = msgctl(queue, IPC_STAT, &buf);
        amount = buf.msg_qnum;

        // if there are no elements to read and 
        // the target data amount has been obtained
        if(amount == 0 && count == DATA_SIZE)
            break;
        
        // recieve message from the queue
        msgrcv(queue, &msg, sizeof(msg), 1, 0); 

        int incoming_data = msg.payload[0];

        count++;

        // write data to file
        fprintf(consumer_file, "%d\n", incoming_data); // write to output file
    }
    
    // purge the queue
    msgctl(queue, IPC_RMID, NULL); 

    // close data file
    fclose(consumer_file);

    return 0; 
} 