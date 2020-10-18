/* listfun.c --- 
 * 
 * 
 * Author: John J. Lass
 * Created: Fri Oct  2 14:29:12 2020 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include "listfun.h" 
#include <stdio.h>
#include <stdlib.h>


void print_node(void *vp){
	if(vp == NULL){
		printf("Passed a null pointer\n");
	}else{
		webpage_t *cp = (webpage_t *) vp;
		printf("INTERNAL URL contained in Queue: %s\n", cp->url);
	}

}


