/* lqget1.test.c --- tests getting from a locked queue
 * 
 * 
 * Author: Robert F. Carangelo III
 * Created: Tue Nov 10 10:43:23 2020 (-0500)
 * Version: 
 * 
 * Description: opens a locked queue, puts to it twice, gets from it and checks to make sure gets the correct element
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
	car_t *car3;

	lqput(lqp,car1);
	lqput(lqp,car2);

	car3 = lqget(lqp);

	if(!(checkcar(car3,"plate1",40.55,2031))) {
		printf("Exit failure!\n");
		exit(EXIT_FAILURE);
	}

	free(car3);
	lqclose(lqp);
	
	exit(EXIT_SUCCESS);
}
