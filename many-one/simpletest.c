#include "thread.h"

void* fun1(void *a){
	printf("In thread 1\n");
    readyprint();
	//printq(*ready);
	for(long i=0; i<10000000; i++);
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
    //while(t1->th_status!= TERMINATED);
    for(long i=0; i<100000200; i++);
	readyprint();
	// void **res;
	// thread_join(1, res);
    
	
	tid2= thread_create(t2, fun2, NULL);
	for(int i=0; i<100000; i++);
	printf("Created: %d\n", tid2);
    return 0;
}