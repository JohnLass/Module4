/* lhapply2.test.c --- 
 * 
 * 
 * Author: Tyler I. Neath
 * Created: Sun Nov 15 19:20:30 2020 (-0500)
 * Version: 
 * 
 * Description: calls haplly on a null Hashtable
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listfun.h"
#include "queue.h"
#include "hash.h"
#include "lhash.h"

int main(void){
	lhashtable_t *testp;
	uint32_t size = 3;

	testp = lhopen(size);
	void (*fn) (void* pt);

	fn = print_node;
	lhapply(testp,fn);

	lhclose(testp);

	exit(EXIT_SUCCESS);
	
}
