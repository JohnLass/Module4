/* lhsearch.test.c --- 
 * 
 * 
 * Author: Tyler I. Neath
 * Created: Sun Nov 15 19:15:04 2020 (-0500)
 * Version: 
 * 
 * Description: 
 * 
 */
#include "hash.h"
#include "listfun.h"
#include "lhash.h"


int main(void){
	uint32_t size = 3;
	lhashtable_t *testp,*resp;
	bool (*fn)(void *cp, const void* keyp);
	testp=lhopen(size);
	car_t *p1 = makecar("keystone",40.4,595);
	lhput(testp,p1,p1->plate,8);
	car_t *p2 = makecar("keystone1",40.5,599);
	lhput(testp,p2,p2->plate,9);
	car_t *p3 = makecar("keystone2",40.6,620);
	lhput(testp,p3,p3->plate,9);

	fn = search_plate;
	resp = lhsearch(testp,fn,"keystone1",9);

	if(!(checkcar(resp,"keystone1",40.5,599))){
		exit(EXIT_FAILURE);
        printf("Test Failed");
    }

	resp = lhsearch(testp,fn,"keystone3",9);

	if(resp!=NULL){
		exit(EXIT_FAILURE);
        printf("Test Failed");
    }
	
	lhclose(testp);

	exit(EXIT_SUCCESS);
}	
	
