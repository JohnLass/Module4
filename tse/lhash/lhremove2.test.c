/* lhremove2.test.c --- 
 * 
 * 
 * Author: Tyler I. Neath
 * Created: Sun Nov 15 19:27:56 2020 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */

#include "lhash.h"
#include "hash.h"
#include "listfun.h"



int main(void){
	uint32_t size = 10;
	lhashtable_t *testp, *resp;
	bool (*fn)(void *cp, const void* keyp);

	
	testp=lhopen(size);
	fn = search_plate;
	resp = lhremove(testp,fn,"keystone1",9);

	if(resp!=NULL)
		exit(EXIT_FAILURE);

	lhclose(testp);
	exit(EXIT_SUCCESS);
}

