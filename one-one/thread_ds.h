#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define STACK_SIZE (1024*64)

typedef struct thread{
    int th_id;
    int th_state;
    //more fields to be added
}thread;

//Singly linked list
typedef struct node{
    thread block;
    struct node *next;
}node;