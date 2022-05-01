#include "thread.h"

void* fun1(void *a){
	printf("In thread 1\n");
    //readyprint();
	//printq(*ready);
	for(long i=0; i<10000000; i++);
	return NULL;
}

void* fun2(void *b){
	printf("In thread 2\n");
	// readyprint();
	return NULL;
}

//Fibonacci Series
void* fibnum(void *y){
	printf("Inside Thread 3\n");
	int i = 0, j = 1, k;
	printf("The Fibonacci series is: ");
	printf("%d %d", i, j);
	for(int n = 2;n < 10; n++){
		k = i + j;
		printf(" %d", k);
		i = j;
		j = k;
	}
	printf("\n");
}

//Matrix Addition
void* matadd(void *z){
	printf("\nInside Thread 4\n");
	int a[4][4] = {{1,2,3,4}, {5,6,7,8}, {1,2,3,4}, {5,6,7,8}};
	int b[4][4] = {{1,2,3,4}, {5,6,7,8}, {1,2,3,4}, {5,6,7,8}};
	int c[4][4] = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}};
	for(int i = 0;i < 4;i++){
		for(int j = 0;j < 4;j++){
			c[i][j] = a[i][j] + b[i][j];
		}
	}
	
	for(int i = 0;i < 4;i++){
		for(int j = 0;j < 4;j++){
			printf("%d\t", c[i][j]);
		}	
		printf("\n");
	}
}

int main(){
	thread_init();
	int tid1, tid2, tid3, tid4;
	thread *t1, *t2, *t3, *t4;

	// creating a thread with null routine pointer
	printf("--------------------------------------------\n");
	printf("Thread create with Null routine pointer: \n");
	if(thread_create(t1, NULL, NULL)){
		printf("Test case passed\n--------------------------------------------\n");
	}
	
	tid1= thread_create(t1, fun1, NULL);
	tid2= thread_create(t2, fun2, NULL);
	tid3= thread_create(t3, fibnum, NULL);
	tid4= thread_create(t4, matadd, NULL);
    //while(t1->th_status!= TERMINATED);
	void ** res;
	//thread_join(1, res);
	printf("main thread\n");
    for(long i=0; i<10000200; i++);
	
    return 0;
}