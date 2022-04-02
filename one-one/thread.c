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
	
	//To be added: allocate stack using mmap
	
	t->th_id= clone(thread_start, t->stack+ STACK_SIZE, CLONE_FS | CLONE_FILES | CLONE_SIGHAND |CLONE_VM, t);
	if(t->th_id== -1){
		free(t);
		printf("Thread creation error\n");
		return errno;
	}
	
}

int thread_start(void *t){
	thread *tcb= (thread*)t;
	//call to sigset context;
	//start the function
	tcb->retval= tcb->function(tcb->arg);
	return 0;
}

int thread_join(thread *tcb);