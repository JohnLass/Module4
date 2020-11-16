/* lhput2.test.c --- 
 * 
 * 
 * Author: Tyler I. Neath
 * Created: Sun Nov 15 19:09:25 2020 (-0500)
 * Version: 
 * 
 * Description: tests hput functionality from lhash.c using a null value
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "listfun.h"
#include "hash.h"
#include "lhash.h"


int main(void){
	uint32_t size = 3;
	int value;
	lhashtable_t *testp;
	testp=lhopen(size);

    // putting in a NULL element
	car_t *p1 = NULL;
	value = lhput(testp,p1,p1->plate,8);
	if(value==0){
		printf("Null test failed");
		exit(EXIT_FAILURE);
	}
    lhclose(testp);
	exit(EXIT_SUCCESS);
	
	
}
