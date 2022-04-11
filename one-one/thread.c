#include "thread_ds.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sched.h>
#include <sys/syscall.h>
#include <unistd.h>

//for singly linked list
node *n = NULL;
thread *tcb = NULL;

int thread_create(thread *tcb, void *(*function) (void *), void *arg){
	//function to be executed
    if(function == NULL){
		printf("Null routine pointer\n");
		return -1;
	}
    //allocate the thread
	thread *t= (thread *)malloc(sizeof(thread));
	tcb = t;
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
	//printf("thread_id: %d", t->th_id);
    //printf("State: %d", t->th_state);
    if(t->th_id== -1){
		free(t);
		printf("Thread creation error\n");
		return errno;
	}

	//Adding thread to linked list
	insert(&n, t);
    //print it
    //traverse(n);
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
    //signal is received
	if(sig > 0){
		kill(thread, sig);
	}
}

int thread_start(void *t){
	thread *tcb= (thread*)t;
	//call to sigset context;
	//start the function
	tcb->retrnval= tcb->function(tcb->arg);
	return 0;
}

int thread_join(thread tcb, void **retrnval){
	//doubt
    thread *t = getnodebytid(tcb, threadid);
    //not found
	if(t == NULL){
		return ESRCH;
	}
    //invalid argument
	if(t->th_state == JOINED) {
        return EINVAL;
    }
    //set the state to JOINED
    if(t->th_state == JOINABLE){
        t->th_state = JOINED;
    }
    //printf("joinstate:%d", t-th_state);
    if(retrnval != NULL){
        retrnval = t->retrnval;
    }
    return -1;
}

//to be done
void thread_exit(){	
    thread *t = getnodebytid(tcb, threadid);
}

//test function
void printarr(int a[]){
    printf("printarr called");
    printf("%d", a[0]);
    printf("%d", a[1]);
    printf("%d", a[2]);
}


int main(){
    int num[3] = {1,2,3};
    thread_create(&tcb, printarr, &num);
    printf("Id: ", n->block-th_id);
    //create join test
    return 0;
}
