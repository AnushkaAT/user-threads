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
	return;

}

thread* dequeue(queue *q){
	if(isempty(*q)==1)
		return 1;
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
	if(q.front== NULL and q.rear== NULL)
		return 1;
	return 0;
}
	
