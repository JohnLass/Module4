/* qsearch2.test.c --- 
 * 
 * 
 * Author: John J. Lass
 * Created: Fri Oct  9 01:32:25 2020 (-0400)
 * Version: 
 * 
 * Description: 
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
	lqueue_t *lq;
	car_t *p1 = makecar("car1", 28.3, 2800);
	car_t *p2 = makecar("car2", 28.3, 800);
	car_t *p3 = makecar("car3", 28.3, 22800);
	bool (*fn)(void *cp, const void* keyp);
	car_t *testp;
	double pricec = 28.3;
	
	fn = search_plate;
	lq = lqopen();
	
	lqput(lq, p1);
	lqput(lq, p2);
	lqput(lq, p3);
	
	fn = search_price;
	testp = (car_t *) lqsearch(lq,fn, &pricec);
	print_node((void *)testp);
	
	if(!(checkcar(testp, "car1", 28.3, 2800))){
		printf("Failed3\n");
		exit(EXIT_FAILURE);
	}

	lqclose(lq);

	
	exit(EXIT_SUCCESS);
}
