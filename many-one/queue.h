#include "thread.h"

//node to implement queue
typedef struct node{
    thread *block;
    struct node *next;
}node;

typedef struct queue{
	node *front, *rear;
	int count;
}queue;

void init_queue(queue *q);
void enqueue(queue *q, thread *t);
thread* dequeue(queue *q);
int isempty(queue q);
void printq(queue q);
thread* searchtid(queue q, int tid);


