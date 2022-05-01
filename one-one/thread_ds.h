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

//Synchronisation
#define LOCKED 1
#define UNLOCKED 0


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
}thread;

//Singly linked list
typedef struct node{
    thread *block;
    struct node *next;
}node;

typedef struct thread_spinlock{
    int flag;
}thread_spinlock;


void insert(node **front, thread *new);
node* searchtid(node *front, int threadid);
node* removenode(node **front, int threadid);

int thread_create(thread *tcb, void *(*function) (void *), void *arg);
void init_lock();
int thread_join(thread tcb, void **retval);


int spinlock_init(thread_spinlock *lock);
int thread_spin_lock(thread_spinlock *lock);
int thread_spin_unlock(thread_spinlock *lock);
