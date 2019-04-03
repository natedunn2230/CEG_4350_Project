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
    key_t key; 
    int msgid; 
    int new_data;

    // set seed for random generator
    srand(time(NULL));

    // ftok to generate unique key 
    key = ftok("progfile", 65); 
  
    // msgget creates a message queue 
    // and returns identifier 
    msgid = msgget(key, 0666 | IPC_CREAT); 

    for(int i = 0; i <DATA_SIZE; i++)
    {
        new_data = rand() % 10000;
        msg.mesg_type = 1; 
        msg.payload[0] = new_data;

        // msgsnd to send message 
        msgsnd(msgid, &msg, sizeof(msg), 0); 

        // display the message 
        printf("Data send is : %d \n", msg.payload[0]); 
  
    }

    return 0; 
} 