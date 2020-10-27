/* indexsave1.test.c --- 
 * 
 * 
 * Author: John J. Lass
 * Created: Tue Oct 27 14:07:23 2020 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "indexio.h"


int main(void){

	int id=1;
	int result;


	result = indexsave(id);

	if(result != 0)
		exit(EXIT_FAILURE);

	exit(EXIT_SUCCESS);

}
