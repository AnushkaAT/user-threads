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
	if(q==NULL)
		return NULL;
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

thread* searchtid(queue q, int tid){
	thread *t;
	node *p;
	p= q.front;
	while(p){
		t= p->block;
		if(t->th_id== tid){
			return t;
		}
		p=p->next;
	}
	return NULL;
}

//function not required for thread. Done only for debugging purpose.
void printq(queue q){
	node *p;
	p= q.front;
	printf("Queue: ");
	while(p){
		printf("%d-> ", p->block->th_id);
		p=p->next;
	}
	printf("\n");
	return;
}

thread* removetid(queue *q, int tid){
	thread *t;
	node *p, *prev=NULL;
	p= q->front;
	while(p){
		t= p->block;
		if(t->th_id== tid){
			if (prev == NULL) {
				q->front= p->next;
	    	} 
			else {
				prev->next = p->next;
	    	}
			t= p->block;
			free(p);
			return t;
		}
		prev= p;
		p=p->next;
	}
	return NULL;
}

/*void insert(klist **front, kthread *new){
    klist *n;
    n = (klist *)(malloc(sizeof(klist)));
    n->block = new;
    if(*front == NULL){
        n->next = NULL;
        *front = n;
        return;
    }
    n->next = *front;
    *front = n;
    return;
}

//Searching a node based on given thread id
klist* searchtid(klist* front, int kid){
    klist *n;
    n = front;
    
    if(n == NULL){
        return NULL;
    }

    while(n != NULL){
        
        if(n->block->kid == kid){
            return n->block;
        }
        n = n->next;
    }
    return NULL;



}

//removing a node with given thread id
node* removenode(klist **front, int kid){
    node *n = *front;

    if(n != NULL && n->block->kid == kid){
        *front = n->next;
        //free(n);
        return n;
    }

    node *p = NULL;
    while(n != NULL && n->block->kid != kid){
        p = n;
        n = n->next;
    }
    
    //node not present
    if(n == NULL){
        return NULL;
    }

    p->next = n->next;
    //free(n);
    return n;
}
*/