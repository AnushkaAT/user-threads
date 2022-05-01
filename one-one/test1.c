#include "thread.c"

void* f1(void *a){
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

void* f2(void *d){
	printf("\nInside Thread2!\n");
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
	thread *t1, *t2;
	thread_create(t1, f1, NULL);
	thread_create(t2, f2, NULL);
	n->block->th_state = JOINABLE;
	thread_join(*n->block, NULL);
	int x = 6;
	thread_exit(&x);
	return 0;
	}
