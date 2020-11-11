/* apply3.test.c --- test for passing null function to qapply 
 * 
 * 
 * Author: Robert F. Carangelo III
 * Created: Sat Oct 10 11:37:02 2020 (-0400)
 * Version: 
 * 
 * Description: Passes a null function to qapply and checks to make sure it gives an error message
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "queue.h"
#include "lqueue.h"
#include "listfun.h"

int main(void){
	lqueue_t *lq;
	car_t *p1 = makecar("car1", 28.3, 2800);
	car_t *p2 = makecar("car2", 128.3, 800);
	car_t *p3 = makecar("car3", 8.3, 22800);
	void (*fn)(void *cp);

	
	fn = NULL;
	lq = lqopen();

	lqput(lq, p1);
	lqput(lq, p2);
	lqput(lq, p3);
	runlqapply(lq, fn);

	
	lqclose(lq);
	
	exit(EXIT_SUCCESS);
}
