/* lqput1.test.c --- tests putting to a locked queue and then closing it
 * 
 * 
 * Author: Robert F. Carangelo III
 * Created: Tue Nov 10 10:07:31 2020 (-0500)
 * Version: 
 * 
 * Description: opens a locked queue, puts to a locked queue twice then closes it
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "queue.h"
#include "lqueue.h"
#include "listfun.h"

int main(void) {
	lqueue_t *lqp = lqopen();
	car_t *car1 = makecar("plate1",40.55,2031);
	car_t *car2 = makecar("plate2",4.5,22031);
	
	lqput(lqp,car1);
	lqput(lqp,car2);

	lqclose(lqp);
	
	exit(EXIT_SUCCESS);
}
