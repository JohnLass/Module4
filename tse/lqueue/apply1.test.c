/* apply1.test.c --- tests apply on a 3 car list
 * 
 * 
 * Author: Robert F. Carangelo III
 * Created: Thu Oct  8 13:34:13 2020 (-0400)
 * Version: 
 * 
 * Description puts three cars in a list, declares print node and passes print node to the qapply funtion, then closes queue: 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "listfun.h"
#include "queue.h"
#include "lqueue.h"

int main(void){
	lqueue_t *lq;
	car_t *p1 = makecar("car1", 28.3, 2800);
	car_t *p2 = makecar("car2", 128.3, 800);
	car_t *p3 = makecar("car3", 8.3, 22800);
	void (*fn)(void *cp);
	car_t *testp;
	
	fn = print_node;
	lq = lqopen();

	lqput(lq, p1);
	lqput(lq, p2);
	lqput(lq, p3);
	lqapply(lq, fn);

	testp = lqget(lq);
	print_node(testp);

	free(testp);
	
	lqclose(lq);
	
	exit(EXIT_SUCCESS);
}
