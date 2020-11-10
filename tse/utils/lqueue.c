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
#include "lqueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <pthread.h>
#include "listfun.h"

typedef struct lqp {
	struct queue_t *queuep;
    pthread_mutex_t lock;
} lqp_t;

/*
 * lqopen -- opens a queue, and makes an associated lock
 */
lqueue_t* lqopen(void) {
    lqueue_t *rtnp;
    queue_t *qp = qopen();
    lqp_t *rp = (lqp_t *) malloc(sizeof(lqp_t));
    rp->queuep = qp;
    pthread_mutex_init(&rp->lock, NULL);
    rtnp = (lqueue_t *) rp;
    return(rtnp);
}  

/*
 * lqclose -- opens a queue, and makes an associated lock
 */
void lqclose(lqueue_t *lqp) {
    if(lqp == NULL){
        return;
    }
    lqp_t *lp = (lqp_t *) lqp;
    qclose(lp->queuep);
    pthread_mutex_destroy(&lp->lock);
    free(lqp);
    return;
}
/* 
 * lqput -- put element at the end of the queue
 * returns 0 is successful; nonzero otherwise 
 */
int32_t lqput(lqueue_t *lqp, void *elementp) {
    if(lqp == NULL || elementp == NULL) {
        return -1;
    }
    lqp_t *lp = (lqp_t *) lqp;
    car_t *car = (car_t*) elementp;
    pthread_mutex_lock(&lp->lock); 
    qput(lp->queuep,elementp);
    printf("car: price: %f\n",car->price);
    printf("In qput: sleeping for 10 second, am locked\n");
    sleep(10);
    pthread_mutex_unlock(&lp->lock);
    return 0;
}

/*
 * lqget -- get the first first element from queue, removing it from the queue 
 * NOTE -> item that gets removed from queue must now be free'd
 */
void* lqget(lqueue_t *lqp) {
    if(lqp == NULL) {
        return NULL;
    }
    lqp_t *lp = (lqp_t*) lqp;
    void *rtn = NULL;
    pthread_mutex_lock(&lp->lock);

    rtn = qget(lp->queuep);
    pthread_mutex_unlock(&lp->lock);
    return rtn;
}

/*
 * rest -- tells the thread to sleep for n seconds
 */
void *rest(void *n) {
    if(n==NULL){
        return NULL;
    }
    int *sec = (int *)n;
    if (*sec<=0) {
        return NULL;
    }
    printf("sleeping for: %d seconds\n", *sec);
    sleep(*sec);
    return NULL;
}


