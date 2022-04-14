#include "thread_ds.h"
#include <stdio.h>

//inserting node at the beginning of the list
void insert(node **front, thread *new){
    node *n;
    n = (node *)(malloc(sizeof(node)));
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
node* searchtid(node *front, int threadid){
    node *n;
    n = front;
    
    if(n == NULL){
        return NULL;
    }

    while(n != NULL){
        
        if(n->block->th_id == threadid){
            return n;
        }
        n = n->next;
    }
    return NULL;



}

//removing a node with given thread id
node* removenode(node **front, int threadid){
    node *n = *front;

    if(n != NULL && n->block->th_id == threadid){
        *front = n->next;
        //free(n);
        return n;
    }

    node *p = NULL;
    while(n != NULL && n->block->th_id != threadid){
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

//Traverse the linked list
void traverse(node *front){
    node *n;
    n = front;
    if(n == NULL){
        printf("The List Is Empty\n");
        return;
    }

    //traverse through the entire list
    while(n != NULL){
        printf("Thread Id: %d\n", n->block->th_id);
        n = n->next;
    }
    return;
}

int main(){
    thread *t1;
    thread *t2;
    thread *t3;
    t1 = (thread *)(malloc(sizeof(thread)));
    t2 = (thread *)(malloc(sizeof(thread)));
    t3 = (thread *)(malloc(sizeof(thread)));
    node *n1;
    n1 = NULL;
    t1->th_id = 100;
    t2->th_id = 400;
    t3->th_id = 2;
    insert(&n1, t1);
    insert(&n1, t2);
    insert(&n1, t3);
    traverse(n1);
}