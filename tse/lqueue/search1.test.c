/* qsearch1.test.c --- 
 * 
 * 
 * Author: John J. Lass
 * Created: Fri Oct  9 00:10:39 2020 (-0400)
 * Version: 
 * 
 * Description: Program to test the funtionality of queue search feature
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "listfun.h"
#include "queue.h"
#include <stdbool.h>
#include "lqueue.h"

int main(void){
	lqueue_t *lq, *lq2;
	car_t *p1 = makecar("car1", 28.3, 2800);
	car_t *p2 = makecar("car2", 128.3, 800);
	car_t *p3 = makecar("car3", 8.3, 22800);
	bool (*fn)(void *cp, const void* keyp);
	car_t *testp;
	
	fn = search_plate;
	lq = lqopen();
	lq2 = lqopen();
	
	lqput(lq, p1);
	lqput(lq, p2);
	lqput(lq, p3);

	if(fn == NULL)
		printf("Fn is null in main\n");
	
	testp = lqsearch(lq,fn,"p6");

	if(testp != NULL){
		printf("failed");
		exit(EXIT_FAILURE);
	}

	print_node((void *)testp);
	lqclose(lq);

	
	testp = lqsearch(lq2,fn,"car3");

	if(testp != NULL){
		printf("failed");
		exit(EXIT_FAILURE);
	}
    print_node((void *)testp);
	lqclose(lq2);
	exit(EXIT_SUCCESS);
}
