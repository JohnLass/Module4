/* thread2.test.c --- 
 * 
 * 
 * Author: Tyler I. Neath
 * Created: Sun Nov 15 21:03:56 2020 (-0500)
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

void *second_thread(void *lhtp) {
	if(lhtp==NULL) {
		return NULL;
	}
	lqueue_t *lp = (lqueue_t*) lhtp;
	car_t *car2 = makecar("plate2",100,200);

	printf("in second thread function, about to put\n");

	lhput(lp,car2,car2->plate,strlen(car2->plate));
	
	return NULL;
}

void *first_thread(void *lhtp) {
	if(lhtp == NULL) {
		return NULL;
	}
	int time = 3;
	lhashtable_t *lp = (lhashtable_t *) lhtp;
	car_t *car1 = makecar("plate1",300,300);
	
	printf("in first thread function, about to sleep\n");
	rest((void *)&time);

	printf("car1: price: %f\n",car1->price);
	printf("in first thread function, about to put\n");
	lhput(lp,car1,car1->plate,strlen(car1->plate));
	return NULL;
}

int main(void) {

	uint32_t size = 3;
	lqueue_t *lhtp = lhopen(size);
	car_t * car3;
	pthread_t tid1, tid2;
	bool (*fn)(void *cp, const void* keyp);

	fn = search_plate;

	printf("In main, about to make a thread\n");
	if(pthread_create(&tid1,NULL,first_thread,lhtp)!=0) {
		printf("thread not created right!\n");
		exit(EXIT_FAILURE);
	}

    printf("In main, about to create another thread and put to it\n");
	if(pthread_create(&tid2,NULL,second_thread,lhtp)!=0) 
		exit(EXIT_FAILURE);

	printf("In main, about to join thread\n");
	if(pthread_join(tid1,NULL) != 0) 
		exit(EXIT_FAILURE);
    
    if(pthread_join(tid2,NULL) != 0) 
		exit(EXIT_FAILURE);


	char *key1 = "plate1";
    char *key2 = "plate2";

    //check to see if the first car is created
	car3 = lhsearch(lhtp,fn,key1,strlen(key1));
    if(!(checkcar(car3,"plate1",300,300))) {
		printf("Not the right car\n");
        exit(EXIT_FAILURE);
	}

    car3 = lhsearch(lhtp,fn,key2,strlen(key2));
    if(!(checkcar(car3,"plate2",100,200))) {
		printf("Not the right car\n");
        exit(EXIT_FAILURE);
	}
	
	printf("In main, closing program\n");

	lhclose(lhtp);
	
	exit(EXIT_SUCCESS);
}

