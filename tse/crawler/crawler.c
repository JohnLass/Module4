/* crawler.c --- first crawler program
 * 
 * 
 * Author: Robert F. Carangelo III
 * Created: Fri Oct 16 15:51:47 2020 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "libutils.a"


int main(void){

	car_t car1 = makecar("Keystone",59.2,0202);
	
	printf("Hello -- Crawler, %d\n",car1->year);
	
	
	exit(EXIT_SUCCESS);

}
