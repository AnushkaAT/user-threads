#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

//Size of stack for a thread
#define STACK_SIZE (1024*64)

typedef struct thread{
    //thread id
    int th_id;

    //thread state
    int th_state;

    //pointer to arguments
    void *args;
    
    //more fields to be added
}thread;

//Singly linked list
typedef struct node{
    thread block;
    struct node *next;
}node;
