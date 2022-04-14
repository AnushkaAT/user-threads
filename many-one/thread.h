#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/mman.h>

//Size of stack for a thread
#define STACK_SIZE (1024*64)

//Maximum threads for each process
#define MAX_THREADS 75

//thread states
#define JOINABLE 1
#define JOINED 2

//thread status: ready, running, terminated
#define READY 1
#define RUNNING 2
#define TERMINATED 3

typedef struct thread{
    //thread id
    int th_id;

    //thread state
    int th_state;
    
    //thread status
    int th_status

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

void thread_init(void);
int thread_create(thread *tcb, void *(*function) (void *), void *arg);
int thread_join(thread *tcb, void **retval);
void thread_exit(void *retval);
int thread_kill(thread tcb, int sig);

void threadt_start(void);
void scheduler(void);

