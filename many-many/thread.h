#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/mman.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdatomic.h>
#include <sys/types.h>

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

#define LOCKED 1
#define UNLOCKED 0

typedef struct thread_spinlock{
    int flag;
}thread_spinlock;

typedef struct thread{
    //user thread id
    int th_id;
    
    //kernel thread id
    int kid;

    //thread state
    int th_state;
    
    //thread status
    int th_status;

    //function pointer
    void *(*function) (void *);
	
    //pointer to arguments
    void *args;
    
    //stack
    void *stack;
    
    //return value
    void *retrnval;
    
    //context of thread-> required for scheduling
    sigjmp_buf context;
    
    //more fields to be added
}thread;

typedef struct kthread{
    int kid;
    thread* running;
    queue *ksched;
}kthread;

void thread_init(int nk);
int thread_create(thread *tcb, void *(*function) (void *), void *arg);
int thread_join(int tid, void **retval);
void thread_exit(void *retval);
void thread_yield(void);
int thread_kill(int tid, int sig);

void thread_start(thread *t);
void scheduler(void);

void block_sig(void);
void unblock_sig(void);
int spinlock_init(thread_spinlock *lock);
int thread_spin_lock(thread_spinlock *lock);
int thread_spin_unlock(thread_spinlock *lock);


void readyprint(void);
