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

	//function pointer
	void *(*function) (void *);
	
    //pointer to arguments
    void *args;
    
    //stack
    void *stack;
    
    //retrun value
    void *retval;
    
    //more fields to be added
}thread;

//Singly linked list
typedef struct node{
    thread block;
    struct node *next;
}node;

void insert(node **front, thread new);
node* searchtid(node *front, int threadid);
node* remove(node **front, int threadid);

int thread_create(thread *tcb, void *(*function) (void *), void *arg);
int thread_join(thread *tcb);
