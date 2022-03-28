#include "thread_ds.h"

//inserting node at the beginning of the list
void insert(node **front, thread new){
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
        
        if(n->block.th_id == threadid){
            return n;
        }
        n = n->next;
    }
    return NULL;



}

node* remove(node **front, int threadid){
    node *n = *front;

    if(n != NULL && n->block.th_id == threadid){
        *front = n->next;
        //free(n);
        return n;
    }

    node *p = NULL;
    while(n != NULL && n->block.th_id != threadid){
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

