/* lqopen.test.c --- tests opening and closing a locked queue
 * 
 * 
 * Author: Robert F. Carangelo III
 * Created: Tue Nov 10 10:01:17 2020 (-0500)
 * Version: 
 * 
 * Description: opens a locked queue then closes it
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "queue.h"
#include "lqueue.h"
#include "listfun.h"
#include <pthread.h>

int main(void) {
	lqueue_t *lqp = lqopen();
	lqclose(lqp);
	
	exit(EXIT_SUCCESS);
}
