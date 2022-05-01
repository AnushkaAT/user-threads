#include "thread.c"

//Fibonacci Series
void* fibnum(void *y){
	printf("Inside Thread1\n");
	int i = 0, j = 1, k;
	printf("The Fibonacci series is: ");
	printf("%d %d", i, j);
	for(int n = 2;n < 10; n++){
		k = i + j;
		printf(" %d", k);
		i = j;
		j = k;
	}
}

//Matrix Addition
void* matadd(void *z){
	printf("\nInside Thread2\n");
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
	init_lock();
	thread *t1, *t2;
	thread_create(t1, fibnum, NULL);
	thread_create(t2, matadd, NULL);
	return 0;
	}
