#include "queue.h"
#include <sys/time.h>
#include <string.h>

#define JB_RSP   6
#define JB_PC    7

//default number of kernel threads for mapping= 3
static int nkthreads= 3;
int thrdcount=0;
//scheduler queue
queue *ready;

void thread_init(void){
    ready= (queue*)malloc(sizeof(queue));
	init_queue(ready);
	//setup signal handler for SIGVTALRM. Signal raised based on working time interupts
	struct sigaction sighand;
	memset (&sighand, 0, sizeof (sighand));
	sighand.sa_handler= &scheduler;
	sighand.sa_flags = 0;
	unblock_sig();
	sigaction(SIGVTALRM, &sighand, NULL);
	
	//setup timer
	//printf("Started timer\n");
	timer_start();
}

void set_nkthreads(int n){
    if(n>1){
        nkthreads= n;
    }
    return;
}

int thread_create(thread *tcb, void *(*function) (void *), void *arg){
    tcb= (thread*)malloc(sizeof(thread));
	if(tcb==NULL){
		printf("ERROR: Memory cannot be allocated\n");
		return -1;
	}
	tcb->function= function;
	tcb->args= arg;
	tcb->th_state = JOINABLE;
	tcb->th_id= thrdcount++;
	tcb->th_status= READY;
	tcb->stack= mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
	if(tcb->stack == MAP_FAILED){
		printf("ERROR: Thread stack allocation\n");
		return -1;
	}
    if(thrdcount< nkthreads){
        //clone(); proper arg here

    }
    enqueue(ready, tcb);
}

int thread_join(int tid, void **retval){

}

void thread_exit(void *retval){

}

void thread_yield(void){

}

int thread_kill(int tid, int sig);

void thread_start(void);
void scheduler(void);

void block_sig(void);
void unblock_sig(void);
int spinlock_init(thread_spinlock *lock);
int thread_spin_lock(thread_spinlock *lock);
int thread_spin_unlock(thread_spinlock *lock);