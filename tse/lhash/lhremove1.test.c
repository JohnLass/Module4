/* lhremove1.test.c --- 
 * 
 * 
 * Author: Tyler I. Neath
 * Created: Sun Nov 15 19:27:47 2020 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */

#include "lhash.h"
#include "hash.h"
#include "listfun.h"



int main(void){
	uint32_t size = 3;
	lhashtable_t *testp,*resp;
	bool (*fn)(void *cp, const void* keyp);
	car_t *p1 = makecar("keystone",40.4,595);
	car_t *p2 = makecar("keystone1",40.5,599);
	car_t *p3 = makecar("keystone2",40.6,620);

	testp=lhopen(size);
	lhput(testp,p1,p1->plate,8);
	lhput(testp,p2,p2->plate,9);
	lhput(testp,p3,p3->plate,9);

	fn = search_plate;
	resp = lhremove(testp,fn,"keystone1",9);
	if(!(checkcar(resp,"keystone1",40.5,599)))
		exit(EXIT_FAILURE);

	resp = lhsearch(testp,fn,"keystone1",9);

	if(resp!=NULL)
		exit(EXIT_FAILURE);

	lhclose(testp);
	free(p2);
	exit(EXIT_SUCCESS);
}	
	
