#include "thread.c"

void *fact(void *a){
	int n = 1;
	for(int i = 1;i <= 10;i++){
		n = n*i;	
	}
	printf("Factorial is: %d\n", n);
	}
	
int main(){
	init_lock();
	thread *t1, *t2, *t3, *t4, *t5;
	thread_create(t1, fact, NULL);
	thread_create(t2, fact, NULL);
	thread_create(t3, fact, NULL);
	thread_create(t4, fact, NULL);
	printf("\nThreads Execution");
	thread_create(t5, fact, NULL);
	//thread_join(*t3, NULL);
	thread_exit(NULL);
	return 0;
	}
