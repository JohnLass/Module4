/* thread2.test.c --- 
 * 
 * 
 * Author: Robert F. Carangelo III
 * Created: Thu Nov 12 00:46:44 2020 (-0500)
 * Version: 
 * 
 * Description: opens one thread, tells it to sleep for 3s, opens another thread, puts to a queue, where it sleeps for 10s and then tries to put to it from the first thread
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "queue.h"
#include "lqueue.h"
#include "listfun.h"
#include <unistd.h>
#include <pthread.h>

void *put_fn(void *lqp) {
	if(lqp==NULL) {
		return NULL;
	}
	lqueue_t *lp = (lqueue_t*) lqp;
	car_t *car1 = makecar("plate1",40.55,2031);

	printf("in second thread function, about to put\n");

	lqput(lp,car1);
	
	return NULL;
}

void *first_thread(void *lqp) {
	if(lqp == NULL) {
		return NULL;
	}
	int time = 2;
	lqueue_t *lp = (lqueue_t *) lqp;
	car_t *car2 = makecar("plate2",56.7,6043);
	
	printf("in first thread function, about to sleep\n");
	rest((void *)&time);

	printf("car2: price: %f\n",car2->price);
	printf("in first thread function, about to put\n");
	lqput(lp,car2);
	free(car2);
	return NULL;
}

int main(void) {
	lqueue_t *lqp = lqopen();
	car_t *car3;
	pthread_t tid1, tid2;


	printf("In main, about to make a thread\n");
	if(pthread_create(&tid1,NULL,first_thread,lqp)!=0) {
		printf("thread not created right!\n");
		exit(EXIT_FAILURE);
	}
	printf("In main, about to create another thread and put to it\n");
	if(pthread_create(&tid2,NULL,put_fn,lqp)!=0) 
		exit(EXIT_FAILURE);
	
	printf("In main, about to join thread\n");
	if(pthread_join(tid1,NULL) != 0) 
		exit(EXIT_FAILURE);
	
	if(pthread_join(tid2,NULL) != 0) 
		exit(EXIT_FAILURE);
	
	car3 = lqget(lqp);
	printf("car3: %f\n",car3->price);
	
	if(!(checkcar(car3,"plate1",40.55,2031))) {
		printf("not the right car\n");
		exit(EXIT_FAILURE);
	}
	free(car3);
	car3 = lqget(lqp);
	
	if(car3 == NULL) {
		printf("car 3 is null\n");
	}
	
	printf("In main, closing program\n");

	lqclose(lqp);
	
	exit(EXIT_SUCCESS);
}
