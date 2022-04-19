#include "queue.h"
#include <sys/time.h>

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
	setitimer(ITIMER_VIRTUAL, &timer, 0);
}

void timer_start(void){
	timer.it_interval.tv_sec= 0;
	timer.it_interval.tv_usec= 10;
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 10;
	setitimer(ITIMER_VIRTUAL, &timer, 0);
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
	//currt->th_status= RUNNING;
	currt->function= currt->args= currt->retrnval= NULL;
	
	//setup signal handler for SIGVTALRM. Signal raised based on working time interupts
	//check if SIGPROF signal is better for implementation->profiling timer
	struct sigaction sighand;
	sighand.sa_handler= scheduler;
	sigaction(SIGVTALRM, &sighand, NULL);
	
	//setup timer
	printf("Started timer\n");
	timer_start();	 
}


int thread_create(thread *tcb, void *(*function) (void *), void *arg){
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
	//tcb->th_status= READY;
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
	
	enqueue(ready, tcb);
	//raise(SIGVTALRM);
	return tcb->th_id;
}

int thread_join(int tid, void **retval){

}

void thread_exit(void *retval){
	if(currt==NULL){
		exit(0);
	}
	currt->retrnval= retval;
	enqueue(complete, currt);
	thread_yield(); 
}

//raise signal, giveup on cpu for other threads
void thread_yield(void){
	raise(SIGVTALRM);
}


int thread_kill(thread tcb, int sig){

}

void thread_start(void){
	void *result= currt->function(currt->args);
	//currt->status= TERMINATED;
	//raise SIGVTALRM signal from thread_exit function;
	thread_exit(result);
}

void scheduler(void){
	printf("Signal: Came to scheduler\n");
	timer_stop();
	//save old context
	sigsetjmp(currt->context, 1);
	
	//round robin fashion for selecting next thread
	enqueue(ready, currt);
	currt= dequeue(ready);
	if(currt== NULL)
		exit(0);
	timer_start();
	siglongjmp(currt->context, 1);
	//wont return here. Jump to function of thread
}


void* fun1(void *a){
	printf("In thread 1\n");
	return NULL;
}

void* fun2(void *b){
	printf("In thread 2\n");
	return NULL;
}

int main(){
	thread_init();
	int tid1, tid2;
	thread *t1, *t2;
	tid1= thread_create(t1, fun1, NULL);
	printf("Created: %d\n", tid1);
	tid2= thread_create(t2, fun2, NULL);
	printf("Created: %d\n", tid2);
	printq(*ready);
	printq(*complete);
	return 0;
}
