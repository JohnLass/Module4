/* lhput1.test.c --- placing an item into the closed hash tabel and try closing it
 * 
 * 
 * Author: Tyler I. Neath
 * Created: Sun Nov 15 18:17:23 2020 (-0500)
 * Version: 1.0
 * 
 * Description: test put functionality for locked hashtabel
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
	car_t *p1 = makecar("keystone",40.4,595);
	value = lhput(testp,p1,p1->plate,8);

	if(value!=0){
		printf("Hash value not entered");
		exit(EXIT_FAILURE);
	}

	lhclose(testp);

	exit(EXIT_SUCCESS);
	
	
}

