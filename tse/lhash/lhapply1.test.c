/* lhapply1.test.c --- testing lhapply
 * 
 * 
 * Author: Tyler I. Neath
 * Created: Sun Nov 15 19:20:16 2020 (-0500)
 * Version: 
 * 
 * Description: puts three cars in a locked hashtable and calls happly
 * 
 */

#include <stdio.h>
#include "hash.h"
#include "listfun.h"
#include <inttypes.h>
#include "lhash.h"

int main(void){
	lhashtable_t *htable;
	uint32_t tsize = 10;
	car_t *car1 = makecar("car1",30.2,8102);
	car_t *car2 = makecar("car2",30.2,8102);
	car_t *car3 = makecar("car3",30.2,8102);
	void (*fn) (void* pt); 
	


	htable = lhopen(tsize);
	lhput(htable, car1, car1->plate,strlen(car1->plate));
	lhput(htable, car3, car3->plate,strlen(car3->plate));
	lhput(htable, car2, car2->plate,strlen(car2->plate));
	fn = print_node;

	lhapply(htable,fn);
	lhclose(htable);

	

	exit(EXIT_SUCCESS);
}
