/* queue.c --- 
 * 
 * 
 * Author: John J. Lass, Tyler Neath, Robert Carangelo
 * Created: Sun Oct  4 14:48:46 2020 (-0400)
 * Version: 1.0
 * 
 * Description:Functions to create and manioulate queues of different data 
 * types
 */

#include "queue.h"
#include "webpage.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

typedef struct ll {
	void *data;
	struct ll *next;
} ll_t;

typedef struct rq {
	struct ll *front;
	struct ll *back;
} rq_t;

/*prints the queue*/
void debugq(queue_t *cp){
	if(cp!=NULL){
		printf("Queue has been created\n");
		rq_t *ptr = (rq_t*)cp;
		printf("This is the front pointer: %p\n",(void *) ptr->front);
		printf("This is the back pointer: %p\n", (void *)ptr->back);
		
		if(ptr->front != NULL){
			ll_t *incp;
			for(incp=ptr->front; incp!=NULL; incp = incp->next){
				printf("This is the ll_t's next: %p\n",(void *)incp->next);
				if(incp->data != NULL){
					printf("Node's data is not empty\n");
				}else{
					printf("Node's data is empty\n");
				}
			}
		}
	}
	else{
		printf("This should point to nil %p\n", cp);
	}
}


/*creates an empty queue,allocates space for a queue, returns the queue in a pointer to a queue_t*/
queue_t* qopen(void) {
	rq_t* ptr=(rq_t*)malloc(sizeof(rq_t));
	ptr->front=NULL;
	ptr->back=NULL;
	return((queue_t*)ptr);
}

/*Deallocates a queue, frees everything in it */
void qclose(queue_t *qp){
	ll_t *hold=NULL;
	if(qp!=NULL){
		rq_t *ptr=(rq_t*)qp;
		//if there is something in the queue
		if(ptr->front!=NULL){
			//loop through the queue, deallocating the memory in each node and the node itself
			do{
				if(ptr->front->data!=NULL){
					free(ptr->front->data);
				}
				hold = ptr->front;
				ptr->front = hold->next;
				free(hold);
			}	while(ptr->front != NULL);
		}	
		//free the queue itself
		free(ptr);
	}
}

/*Put element at the end of the queue
return 0 if successful, nonzero otherwise
allocate space in queue for new item*/
int32_t qput(queue_t *qp, void *elementp){
	int32_t rtrn = 1;
	
	if(qp!=NULL){
		if(elementp!=NULL){
			//if both arguments are valid, need to cast the qp and allocate space for new node
			rq_t *ptr = (rq_t*)qp;
			ll_t *newp;
			if( (newp = (ll_t*) malloc(sizeof(ll_t))) == NULL)
				return rtrn;
			newp->data = elementp;
			newp->next = NULL;
			//if it isnt the first thing reassign the back thing in list to point to new one and reassign back pointer
			if(ptr->front != NULL){
				ptr->back->next = newp;
				ptr->back = newp;
			}else{
				//set front and back to new thing
				ptr->front = newp;
				ptr->back = newp;			 
			}
			rtrn = 0;
		}
	}
	return rtrn;
}


/*get the first element from the queue, remove it from the queue, free memory to its spot in the queue but not the actual data*/
void* qget(queue_t *qp){
	void *rtrn = NULL;	
	if(qp!=NULL){
		rq_t *ptr = (rq_t *) qp;
		if(ptr->front != NULL){
			ll_t *hold = NULL;
			hold = ptr->front;
			if(ptr->front == ptr->back)
				ptr->back = NULL;
			ptr->front = hold->next;
			rtrn = hold->data;
			free(hold);
		}else{
			printf("queue is empty\n");
		}
	}
	return rtrn;
}


/*apply a function to every element in the queue*/
void qapply(queue_t *qp, void (*fn)(void* elementp)){
	if(qp!=NULL){
		if(fn!=NULL){
			rq_t *ptr = (rq_t *) qp;
			if(ptr->front!=NULL){
				ll_t *incp = NULL;
				for(incp=ptr->front; incp!=NULL; incp=incp->next){
					fn(incp->data);
				}
			}
			else{
				printf("Queue is empty!\n");	
			}
		}
		else{
			printf("Passed Null function!\n");
		}
	}
		else{
		printf("Queue does not exist!\n");
	}
}



void* qsearch(queue_t *qp,bool (*searchfn)(void* elementp,const void* keyp),const void* skeyp){
	if(qp != NULL){
		rq_t *ptr=(rq_t*)qp;
		ll_t *incp;
		bool flag;

		if(ptr->front!=NULL){
			for(incp=ptr->front;incp!=NULL;incp=incp->next){
				flag = searchfn(incp->data,skeyp);
				
				if(flag==true){
					//	printf("Element Found!\n");
					return incp->data;
				}
			}
		}
		//if(ptr->front==NULL)
			//printf("Empty List");
			//	printf("Element Not Found.\n");
		return NULL;
	}
	printf("Invalid arg\n");
	return NULL;
}



void* qremove(queue_t *qp,
							bool (*searchfn)(void* elementp,const void* keyp),
							const void* skeyp){
	void * rtn = NULL;
	if(qp != NULL){
		rq_t *ptr=(rq_t*)qp;
		ll_t *incp, *prev, *hold;
		bool flag = false;
		int loop_count = 0;
		hold = NULL;
		
		if(ptr->front!=NULL){
			incp = ptr->front;
			while(flag == false && incp != NULL){		
				flag = searchfn(incp->data,skeyp);
				printf("Loop\n");
				if(flag==true){
					printf("Element Found!\n");
					rtn =  incp->data;
					if(loop_count == 0){
						ptr->front = incp->next;
					}else{
						prev->next = incp->next;
					}
					hold = incp;
					
					free(hold);
				}else{
					prev = incp;
					incp = incp->next;
				}
				loop_count = loop_count +1;
			}
			
		}else
			printf("Empty List");
		if(flag == 0)
			printf("Element Not Found.\n");
	}
	return rtn;
}


/*combines q2 into the end of q1, then frees q2*/
void qconcat(queue_t *q1p, queue_t *q2p){
	if(q1p != NULL && q2p != NULL){
		rq_t *ptr1 = (rq_t *) q1p;
		rq_t *ptr2 = (rq_t *) q2p;
		if(ptr2->back != NULL){
			if(ptr1->front == NULL){
				ptr1->front = ptr2->front;
			}else{
				ptr1->back->next = ptr2->front;
			}
			ptr1->back = ptr2->back;
			ptr2->back = NULL;
			ptr2->front = NULL;
		}else{
			printf("Second queue is empty!\n");
		}
		qclose(ptr2);
	}else{
		printf("Passed null pointer!\n");
	}
}
