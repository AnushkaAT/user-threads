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
	thread *t1, *t2, *t3, *t4;

	// creating a thread with null routine pointer
	if(thread_create(t1, NULL, NULL)){
		printf("Thread create with Null routine pointer: \n");
		printf("Test case passed\n");
	}
	
	tid1= thread_create(t1, fun1, NULL);
	tid2= thread_create(t2, fun2, NULL);
    //while(t1->th_status!= TERMINATED);
    for(long i=0; i<100000200; i++);
	printf("for loop ends\n");
	for(int i=0; i<100000; i++);
    return 0;
}