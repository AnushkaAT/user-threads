#define _GNU_SOURCE
#include <sched.h>
#include "thread_ds.c"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sched.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/mman.h>

//for singly linked list
node *n = NULL;
thread *tcb = NULL;
thread_spinlock l;
//thread count
static int count = 0;

void init_lock(){

	spinlock_init(&l);
	//printf("%d", l.flag);
	return;
}

int thread_start(void *t){
	thread *tcb = NULL;
	tcb = (thread*)t;
	tcb->retrnval= tcb->function(tcb->args);
	return 0;
}

int thread_create(thread *tcb, void *(*function) (void *), void *arg){

    //function to be executed
    if(function == NULL){
		printf("Null routine pointer\n");
		return -1;
    }
    //allocate the thread
	thread *t= (thread *)malloc(sizeof(thread));
	if(t == NULL){
		printf("Memory cannot be allocated\n");
		return -1;
	}
	tcb = t;
	t->function= function;
	t->args= arg;
	t->th_state = JOINABLE;
	
	t->stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
	if(t->stack == MAP_FAILED){
		printf("Thread stack allocation error\n");
		return -1;
	}
	t->th_id= clone(thread_start, t->stack + STACK_SIZE, SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND |CLONE_VM, t);
	
    //printf("thread_id: %d", t->th_id);
    if(t->th_id == -1){
		free(t);
		printf("Thread creation error\n");
		return -1;
	}
	thread_spin_lock(&l);
	insert(&n, t);
	
	//Thread count
	count++;
	thread_spin_unlock(&l);
	return 1;
}

int thread_kill(int tid, int sig){

   if(sig < 0 || sig > 64){
   	return EINVAL;
   }	
   
   //no signal
   else if(sig == 0){
   	return 0;	
   }
   else{
   	kill(tid, sig);
   }
}

//Cleaning Threads
void thread_clean(){
	
	//remove first node
	while(count > 0){
		removenode(&n, n->block->th_id);
		count--;
	}
	printf("Cleanup done");
	return;
}

int thread_join(thread tcb, void **retrnval){

    int id = tcb.th_id;
    int i;
    //printf("join id %d", id);
    if(id < 0){
    	return -1;
    }
    thread_spin_lock(&l);
    node *t = searchtid(n, id);
    //printf("/nBefore Join:%d", t->block->th_state);
    if(t == NULL){
	return -1;
	thread_spin_unlock(&l);
     }
     thread_spin_unlock(&l);
     
    int val = (t->block->th_state != JOINED && t->block->th_state == JOINABLE) ? 1 : 0;
    if(val == 0) {
        return EINVAL;
    }
    else{
        t->block->th_state = JOINED;
        waitpid(id, &i, 0);
        if(i == 1){
        	printf("Waitpid Error");
        	return -1;
        }        
    }
    
    //printf("/nAfter Join:%d", t->block->th_state);
    if(retrnval != NULL){
        retrnval = t->block->retrnval;
    }
    
    return 0;
}

//Calling thread terminated
void thread_exit(void *retrnval){
	
	//tid of calling thread
	int id = gettid();
	
	//printf("Caller ThreadPid=%d", id);
	node *t = searchtid(n, id);
	
	if(t == NULL){
		thread_spin_unlock(&l);
		return;
	}
	
	t->block->retrnval = retrnval;
	
	kill(id, SIGKILL);
	
	//printf("Success");
	return;
}

//Initialise spinlock
int spinlock_init(thread_spinlock *lock){
	lock= (thread_spinlock *)malloc(sizeof(thread_spinlock));
	lock->flag= UNLOCKED;
	return 0;
}

int thread_spin_lock(thread_spinlock *lock){

	while(__sync_lock_test_and_set(&(lock->flag),1))
		;
	return 0;
}

int thread_spin_unlock(thread_spinlock *lock){

	if(lock->flag == 1) {
        __sync_lock_release(&(lock->flag),0);
        return 0;
    }
    return EINVAL;
}
