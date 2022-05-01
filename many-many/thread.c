#include "queue.h"
#include <sys/time.h>
#include <string.h>

#define JB_RSP   6
#define JB_PC    7

//default number of kernel threads for mapping= 3
static int nkthreads= 3;
int thrdcount=0;
//ktlist is a global array of kernel threads
kthread *ktlist;

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

void timer_start(void){
	//printf("In timer start function\n");
	timer.it_interval.tv_sec= 0;
	timer.it_interval.tv_usec= 10000;
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 10000;
	setitimer(ITIMER_VIRTUAL, &timer, NULL);
}

void thread_init(int nk){
	if(n>1){
        nkthreads= n;
    }
	//initialise the global array of kernel threads.
	//inefficient: space may get wasted! Make shift work for now
    ktlist= (kthread*)malloc(sizeof(kthread)*nkthreads);
	for(int i=0; i<nkthreads; i++){
		ktlist[i].running=NULL;
		ktlist[i].ksched= (queue*)malloc(sizeof(queue));
		init_queue(ktlist[i].ksched);
	}
	//setup signal handler for SIGVTALRM.
	struct sigaction sighand;
	memset (&sighand, 0, sizeof (sighand));
	sighand.sa_handler= &scheduler;
	sighand.sa_flags = 0;
	unblock_sig();
	sigaction(SIGVTALRM, &sighand, NULL);
	//setup timer
	timer_start();
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
	sigsetjmp(currt->context, 1);
	//modify rsp(index JB_RSP=6) and pc(index JB_PC=7) in sigjmp_buf. Mangle it first
	tcb->context[0].__jmpbuf[JB_RSP] = mangle((long int) tcb->stack+ STACK_SIZE - sizeof(long int));
	tcb->context[0].__jmpbuf[JB_PC] = mangle((long int) thread_start);

	tcb->stack= mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
	if(tcb->stack == MAP_FAILED){
		printf("ERROR: Thread stack allocation\n");
		return -1;
	}
    if(thrdcount< nkthreads){
        //clone(); proper arg here
		tcb->kid= clone(thread_start, t->stack+ STACK_SIZE, SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND |CLONE_VM, tcb);
		timer_start();
    }
	else{
		int index= (tcb->th_id)%nkthreads;
		enqueue(ktlist[index].ksched, tcb);
		scheduler();
	}
}

int thread_join(int tid, void **retval){

}

void thread_exit(void *retval){

}

void thread_yield(void){

}

int thread_kill(int tid, int sig);

void thread_start(thread *t){
	thread *tcb= (thread*)t;
	//call to sigset context;
	//start the function
	tcb->retrnval= tcb->function(tcb->args);
}
void scheduler(void);

void block_sig(void);
void unblock_sig(void);
int spinlock_init(thread_spinlock *lock);
int thread_spin_lock(thread_spinlock *lock);
int thread_spin_unlock(thread_spinlock *lock);