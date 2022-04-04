#include "thread_ds.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sched.h>
#include <sys/syscall.h>
#include <unistd.h>

int thread_create(thread *tcb, void *(*function) (void *), void *arg){
	if(function== NULL){
		printf("Null routine pointer\n");
		return -1;
	}
	thread *t= (thread*)malloc(sizeof(thread));
	tcb= t;
	if(t==NULL){
		printf("Memory cannot be allocated\n");
		return -1;
	}
	
	t->function= function;
	t->arg= arg;
	t->th_state = JOINABLE;
	
	//To be added: allocate stack using mmap
	t->stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
	if(t->stack == MAP_FAILED){
		printf("Thread stack allocation error\n");
		return -1;
	}
	
	t->th_id= clone(thread_start, t->stack+ STACK_SIZE, SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND |CLONE_VM, t);
	if(t->th_id== -1){
		free(t);
		printf("Thread creation error\n");
		return errno;
	}
	//Adding thread to linked list
	insert();
}

int thread_kill(thread tcb, int sig){
	//if sig=0, no signal sent
    if(sig == 0) {
        return 0;
    }
	//check for signal
	if(sig < 0 || sig > 32){
		return EINVAL;	
	}	

	if(sig > 0){
		kill(thread, sig);
	}
}

int thread_start(void *t){
	thread *tcb= (thread*)t;
	//call to sigset context;
	//start the function
	tcb->retval= tcb->function(tcb->arg);
	return 0;
}

int thread_join(thread tcb){
	thread *t = getnodebytid(thread, threadid);
	if(t == NULL){
		return ESRCH;
	}
	if(t->th_state == JOINED) {
        return EINVAL;
    }

}

void thread_exit(){	
}
