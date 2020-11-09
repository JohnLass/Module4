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
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

typedef struct lqp {
	struct queue_t *queuep;
    pthread_mutex_t lock;
} lqp_t;

/*
 * lqopen -- opens a queue, and makes an associated lock
 */
lqueue_t* lqopen(void) {
    queue_t *qp = qopen();
    lqp_t *rp = (lqp_t *) malloc(sizeof(lqp_t));
    rp->queuep = qp;
    pthread_mutex_init(&rp->lock, NULL);
    return(rp);
}  

/*
 * lqclose -- opens a queue, and makes an associated lock
 */
void lqclose(queue_t *lqp) {
    if(lqp == NULL){
        return;
    }
    lqp_t *lp = (lqp_t *) lqp;
    qclose(lp->queuep);
    pthread_mutex_destroy(&lp->lock);
    return;
}
