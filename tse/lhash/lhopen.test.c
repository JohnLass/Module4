/* lhopen.test.c --- tests opening and closing a locked hash table
 * 
 * 
 * Author: Tyler I. Neath
 * Created: Thu Nov 12 3:57:17 2020
 * Version: 
 * 
 * Description: Opens a locked hashtable and closes it 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "queue.h"
#include "lhash.h"
#include "listfun.h"
#include <pthread.h>

int main(void) {
	lhashtable_t *lht = lhopen(100);
	lhclose(lht);
	exit(EXIT_SUCCESS);
}
