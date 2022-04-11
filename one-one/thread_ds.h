#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

//Size of stack for a thread
#define STACK_SIZE (1024*64)

//Maximum threads for each process
#define MAX_THREADS 75

//thread states
#define JOINABLE 1
#define JOINED 2

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
    
    //return value
    void *retrnval;
    
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
