#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void init_queue(queue *q){
	q->front=q->rear= NULL;
	q->count=0;
	return;
}

void enqueue(queue *q, thread *t){
	node *p= (node*)malloc(sizeof(node));
	p->block= t;
	p->next= NULL;
	if(q->count==0){
		q->front= q->rear= p;
	}
	else{
		q->rear->next= p;
		q->rear=p;
	}
	(q->count)++;
	//printf("enqued: %d\n", q->count);
	return;

}

thread* dequeue(queue *q){
	if(isempty(*q)==1)
		return NULL;
	thread *t;
	node *p;
	p= q->front;
	t= p->block;
	q->front= q->front->next;
	(q->count)--;
	free(p);
	return t;
}

int isempty(queue q){
	if(q.front== NULL && q.rear== NULL)
		return 1;
	return 0;
}

//function not required for thread. Done only for debugging purpose.
void printq(queue q){
	node *p;
	p= q.front;
	printf("Queue:\n");
	while(p){
		printf("%d-> ", p->block->th_id);
		p=p->next;
	}
	printf("\n");
	return;
}

//just to ensure queue is working properly
/*int main(){
	queue *q= (queue*)malloc(sizeof(queue));
	init_queue(q);
	thread t1, t2, t3;
	t1.th_id= 1; t2.th_id= 2; t3.th_id= 3;
	enqueue(q, &t1);
	enqueue(q, &t2);
	enqueue(q, &t3);
	printf("Count: %d\n", q->count);
	printq(*q);
	return 0;
}*/

int main(){
return 0;
}
