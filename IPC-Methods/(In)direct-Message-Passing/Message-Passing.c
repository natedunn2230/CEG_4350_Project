#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h> 
#include <sys/msg.h> 
#include <time.h>

#define DATA_SIZE 100

void ProducerJob();
void ConsumerJob();

// structure for message queue 
struct mesg_buffer { 
    long mesg_type; 
    int mesg[1];
} message; 

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
        ProducerJob();
    }
    else 
    {
        // child process takes role of the consumer
        ConsumerJob();
    }

    return 0;
}

/*
Produces 100 random integer values, and passes them indiviually to
the message queue for the consumer to read from
*/
void ProducerJob()
{
    // set seed for random generator
    srand(time(NULL));

 
    // limit integer value to 10000 to improve readability
    int new_data = rand() % 10000;
    
    key_t key; 
    int msgid; 

    // ftok to generate unique key 
    key = ftok("progfile", 65); 

    // msgget creates a message queue 
    // and returns identifier 
    msgid = msgget(key, 0666 | IPC_CREAT); 
    message.mesg_type = 1; 
    message.mesg[0] = new_data;

    // msgsnd to send message 
    msgsnd(msgid, &message, sizeof(message), 0); 

    // display the message 
    printf("Data send is : %d \n", message.mesg[0]); 

}

/*
Reads 100 random integers from the message queue that were produced
by the producer process
*/
void ConsumerJob()
{
    key_t key; 
    int msgid; 

    // ftok to generate unique key 
    key = ftok("progfile", 65); 

    // msgget creates a message queue 
    // and returns identifier 
    msgid = msgget(key, 0666 | IPC_CREAT); 

    // msgrcv to receive message 
    msgrcv(msgid, &message, sizeof(message), 1, 0); 

    // display the message 
    printf("Data Received is : %d \n",  
                    message.mesg[0]); 

    // to destroy the message queue 
    msgctl(msgid, IPC_RMID, NULL); 

}

