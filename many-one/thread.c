#include <queue.h>
#include <sys/time.h>

//global variables
static queue *schedq; //queue of thread for scheduling
static thread *currt; //currently running thread
static int thrdcount= 0; //current 
static struct itimerval timer;

//mangle: encryption for pointers in jmp_buf array. 
//Function mangle taken from https://sites.cs.ucsb.edu/~chris/teaching/cs170/projects/proj2.html
static long int i64_ptr_mangle(long int p){
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

void thread_init(void){
	schedq= (queue*)malloc(sizeof(queue));
	init_queue(schedq);
	//main thread
	currt= (thread*)malloc(sizeof(thread));
	currt->th_id= thrdcount++;
	currt->th_state= JOINABLE;
	currt->th_status= RUNNING;
	currt->function= currt->args= currt->retrnval= NULL;
	
	//setup signal handler for SIGVTALRM. Signal raised based on working time interupts
	struct sigaction sighand;
	sighand.sa_handler= scheduler
	sigaction(SIGVTALRM, &sighand, NULL);
	
	//setup timer
	timer.it_interval.tv_sec= 0;
	timer.it_interval.tv_usec= 10;
	timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 10;
    setitimer(ITIMER_VIRTUAL, timer, 0);
	 
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
	tcb->th_status= READY;
	tcb->stack= mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
	if(tcb->stack == MAP_FAILED){
		printf("Thread stack allocation error\n");
		return -1;
	}
	//context
	sigset(tcb->context, 1);
	//modify rsp(index JB_RSP=6) and pc(index JB_PC=7) in sigjmp_buf. Mangle it first
	tcb->context[0].__jmpbuf[JB_SP] = mangle((long int) tcb->stack+ STACKSIZE - sizeof(long int));
	tcb->context[0].__jmpbuf[JB_PC] = mangle((long int) thread_start);
	
	enqueue(schedq, tcb);
	return 0;
}

int thread_join(thread *tcb, void **retval){

}

void thread_exit(void *retval){

}

int thread_kill(thread tcb, int sig){

}

void thread_start(void){
	currt->retrnval= currt->function(currt->args);
	currt->status= TERMINATED;
	//raise SIGVTALRM signal;
}

void scheduler(void){

}
