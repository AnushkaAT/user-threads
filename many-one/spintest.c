#include "thread.h"
//test case: race condition discussed in synchronization lecture
static long c=0, c1=0, c2=0, run=1;
thread_spinlock l;
//l= (thread_spinlock*)malloc(sizeof(thread_spinlock));

void* inc1(void *a){
    while(run==1){
        thread_lock(&l);
        c++;
        thread_unlock(&l);
        c1++;
    }
}

void * inc2(void *a){
    while(run==1){
        thread_lock(&l);
        c++;
        thread_unlock(&l);
        c2++;
    }
}


int main(){
    thread_init();
    l.flag=0;
    //printf("%d\n", l.flag);
	int tid1, tid2;
    thread *t1, *t2;
    tid1= thread_create(t1, inc1, NULL);
	tid2= thread_create(t2, inc2, NULL);
    for(long i=0; i<100000000; i++);
    //sleep(2);
    run = 0;
    fprintf(stdout, "c = %ld c1+c2 = %ld c1 = %ld c2 = %ld \n", c, c1+c2, c1, c2);
    fflush(stdout);
}