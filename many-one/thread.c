#include "queue.h"
#include <sys/time.h>
#include <string.h>

#define JB_RSP   6
#define JB_PC    7

//global variables
static queue *ready, *complete; //queue of thread for scheduling
static thread *currt; //currently running thread
static int thrdcount= 0; //current 
static struct itimerval timer;

//mangle: encryption for pointers in jmp_buf array. 
//Function mangle taken from https://sites.cs.ucsb.edu/~chris/teaching/cs170/projects/proj2.html
static long int mangle(long int p){
	long int ret;
	asm(" mov %1, %%rax;\n"
		" xor %%fs:0x30, %%rax;"
		" rol $0x11, %%rax;"
		" mov %%rax, %0;"
	: "=r"(ret)
	: "r"(p)
	: "%rax"
	);
	return ret;
}

void timer_stop(void){
	timer.it_interval.tv_sec= 0;
	timer.it_interval.tv_usec= 0;
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 0;
	setitimer(ITIMER_VIRTUAL, &timer, NULL);
}

void timer_start(void){
	//printf("In timer start function\n");
	timer.it_interval.tv_sec= 0;
	timer.it_interval.tv_usec= 10000;
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 10000;
	setitimer(ITIMER_VIRTUAL, &timer, NULL);
}


void thread_init(void){
	ready= (queue*)malloc(sizeof(queue));
	init_queue(ready);
	complete= (queue*)malloc(sizeof(queue));
	init_queue(complete);
	//main thread
	currt= (thread*)malloc(sizeof(thread));
	currt->th_id= thrdcount++;
	currt->th_state= JOINABLE;
	currt->th_status= RUNNING;
	currt->function= currt->args= currt->retrnval= NULL;
	sigsetjmp(currt->context, 1);
	enqueue(ready, currt);
	//setup signal handler for SIGVTALRM. Signal raised based on working time interupts
	//check if SIGPROF signal is better for implementation->profiling timer
	struct sigaction sighand;
	 memset (&sighand, 0, sizeof (sighand));
	sighand.sa_handler= &scheduler;
	sighand.sa_flags = 0;
	sigaction(SIGVTALRM, &sighand, NULL);
	
	//setup timer
	//printf("Started timer\n");
	timer_start();
}


int thread_create(thread *tcb, void *(*function) (void *), void *arg){
	//timer_stop();
	block_sig();
	//function to be executed
    if(function == NULL){
		printf("Null routine pointer\n");
		return -1;
	}
	tcb= (thread*)malloc(sizeof(thread));
	if(tcb==NULL){
		printf("Memory cannot be allocated\n");
		return -1;
	}
	tcb->function= function;
	tcb->args= arg;
	tcb->th_state = JOINABLE;
	tcb->th_id= thrdcount++;
	tcb->th_status= READY;
	tcb->stack= mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
	if(tcb->stack == MAP_FAILED){
		printf("Thread stack allocation error\n");
		return -1;
	}
	//context
	sigsetjmp(tcb->context, 1);
	//modify rsp(index JB_RSP=6) and pc(index JB_PC=7) in sigjmp_buf. Mangle it first
	tcb->context[0].__jmpbuf[JB_RSP] = mangle((long int) tcb->stack+ STACK_SIZE - sizeof(long int));
	tcb->context[0].__jmpbuf[JB_PC] = mangle((long int) thread_start);
	//printf("%p\n", tcb->context);
	enqueue(ready, tcb);
	//printq(*ready);
	unblock_sig();
	timer_start();
	//raise(SIGPROF);
	return tcb->th_id;
}

int thread_join(int tid, void **retval){
	//timer_stop();
	block_sig();
	if(tid<0){
		unblock_sig();
		timer_start();
		errno= EINVAL;
		return -1;
	}
	if(currt->th_id== tid){
		unblock_sig();
		timer_start();
		//deadlock
		return -1;
	}
	
	thread *t= searchtid(*ready, tid);
	if(t==NULL){
		unblock_sig();
		timer_start();
		return -1;
	}
	if(t->th_state == JOINED) {
		unblock_sig();
		timer_start();
		return EINVAL;
    }
    t->th_state= JOINED;
	printf("Join: waiting\n");
	unblock_sig();
    timer_start();
    while(t->th_status!= TERMINATED);
	printf("%d terminated\n", t->th_id);
    if(t){
    	*retval= t->retrnval;
    }
	return 0;
}

void thread_exit(void *retval){
	printf("Exiting thread: %d\n", currt->th_id);
	block_sig();
	if(currt==NULL){
		exit(0);
	}
	currt->th_status= TERMINATED;
	currt->retrnval= retval;
	enqueue(complete, currt);
	currt= dequeue(ready);
	printq(*ready);
	unblock_sig();
	thread_yield(); 
}

//raise signal, giveup on cpu for other threads
void thread_yield(void){
	raise(SIGVTALRM);
}


int thread_kill(int tid, int sig){
	//invalid signal
	if(sig<0 || sig>64)
		return EINVAL; 

	if(tid== currt->th_id){
		printf("raised signal %d to tid %d", sig, tid);
		raise(sig);
	}


}

void thread_start(void){
	unblock_sig();
	void *result= currt->function(currt->args);
	//currt->status= TERMINATED;
	//raise SIGVTALRM signal from thread_exit function;
	thread_exit(result);
}

void scheduler(void){
	printf("Signal: Came to scheduler: %d\n", currt->th_id);
	timer_stop();
	//save old context
	if(currt== NULL){
		printf("currt null\n");
		return;
	}
	if(sigsetjmp(currt->context, 1)== 1)
		return;

	if(currt->th_status!= TERMINATED)
		currt->th_status= READY;
	//round robin fashion for selecting next thread
	enqueue(ready, currt);
	printf("enqueued %d in scheduler\n", currt->th_id);
	currt= dequeue(ready);
	if(currt== NULL)
		exit(0);
	timer_start();
	//printf("timer started, now longjmp\n");
	//printf("context: %p\n", currt->context);
	siglongjmp(currt->context, 1);
	//wont return here. Jump to function of thread
}


void block_sig(void){
	sigset_t siga;
    sigemptyset(&siga);
    sigaddset(&siga, SIGVTALRM);

    if (sigprocmask(SIG_BLOCK, &siga, NULL) == -1) {
        perror("sigprocmask");
        exit(0);
    }
    return;
}

void unblock_sig(void){
	sigset_t siga;
    sigemptyset(&siga);
    sigaddset(&siga, SIGVTALRM);

    if (sigprocmask(SIG_UNBLOCK, &siga, NULL) == -1) {
        perror("sigprocmask");
        exit(0);
    }
    return;
}


//synchronization: spinlocks
int spinlock_init(thread_spinlock *lock){
	*lock= 0;
	return 0;
}

int thread_spin_lock(thread_spinlock *lock){
	while(__sync_lock_test_and_set(lock,1))
		;
	return 0;
}

int thread_spin_unlock(thread_spinlock *lock){
	if(*lock == 1) {
        __sync_lock_release(lock,0);
        return 0;
    }
    return EINVAL;
}

void readyprint(void){
	printq(*ready);
}