#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h> 
#include <sys/ipc.h> 
#include <sys/msg.h>
#include <time.h> 

// structure for the message
struct buffer { 
    long mesg_type; 
    int payload[1]; 
} msg; 

int main() 
{ 
    key_t key; 
    int msgid; 
  
    // ftok to generate unique key 
    key = ftok("progfile", 65); 
  
    // msgget creates a message queue 
    // and returns identifier 
    msgid = msgget(key, 0666 | IPC_CREAT); 
  
    // msgrcv to receive message 
    msgrcv(msgid, &msg, sizeof(msg), 1, 0); 
  
    // display the message 
    printf("Data Received is : %d \n",  
                    msg.payload[0]); 
  
    // to destroy the message queue 
    msgctl(msgid, IPC_RMID, NULL); 
  
    return 0; 
} 