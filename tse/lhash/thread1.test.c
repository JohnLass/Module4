/* thread1.test.c --- 
 * 
 * 
 * Author: John J. Lass
 * Created: Sun Nov 15 19:55:59 2020 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "queue.h"
#include "lqueue.h"
#include "lhash.h"
#include "hash.h"
#include "listfun.h"
#include <unistd.h>
#include <pthread.h>

void *first_thread(void *lqp) {
	if(lqp == NULL) {
		return NULL;
	}
	int time = 3;
	lhashtable_t *lp = (lhashtable_t *) lqp;
	car_t *car2 = makecar("plate2",56.7,6043);
	
	printf("in first thread function, about to sleep\n");
	rest((void *)&time);

	printf("car2: price: %f\n",car2->price);
	printf("in first thread function, about to put\n");
	lhput(lp,car2,car2->plate,strlen(car2->plate));
	return NULL;
}

int main(void) {

	uint32_t size = 3;
	lqueue_t *lqp = lhopen(size);
	car_t *car3;
	pthread_t tid1;
	bool (*fn)(void *cp, const void* keyp);

	fn = search_plate;

	printf("In main, about to make a thread\n");
	if(pthread_create(&tid1,NULL,first_thread,lqp)!=0) {
		printf("thread not created right!\n");
		exit(EXIT_FAILURE);
	}

	printf("In main, about to join thread\n");
	if(pthread_join(tid1,NULL) != 0) 
		exit(EXIT_FAILURE);

	char *key = "plate2";

	car3 = lhsearch(lqp,fn,key,strlen(key));
	printf("car3: %f\n",car3->price);
	lhremove(lqp,fn,key,strlen(key));

	if(car3 == NULL) {
		printf("car3 is null\n");
	}
	
	if(!(checkcar(car3,"plate2",56.7,6043))) {
		printf("not the right car\n");
		exit(EXIT_FAILURE);
	}
	free(car3);
	car3 = lhsearch(lqp,fn,key,strlen(key));
	lhremove(lqp,fn,key,strlen(key));
	
	if(car3 == NULL) {
		printf("car 3 is null\n");
	}
	
	printf("In main, closing program\n");

	lhclose(lqp);
	
	exit(EXIT_SUCCESS);
}
