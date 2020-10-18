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

#include "webpage.h"
#include "hash.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main(void){
	char *url = "https://thayer.github.io/engs50/";
	int depth = 0;
	
	webpage_t *w1 = webpage_new(url,depth,NULL);
	webpage_t *HOLD;

	if(webpage_fetch(w1)){
		printf("Webpage found and fetched\n");
	} else{
		printf("Error getting webpage");
		exit(EXIT_FAILURE);
	}

	
	// iterating over URLS
	qopen();
	int pos=0;
	char *result;
	while((pos=webpage_getNextURL(w1, pos, &result)) > 0){

		if(!(strncmp(url,result,32))){
		printf("Found Internal URL: %s\n", result);
		HOLD = webpage_new(url,depth,NULL);
		qput(HOLD);
		}else{
			printf("Found External URL: %s\n", result);
		}
		
		free(result);
	}

	qapply(printnode());
	webpage_delete(w1);
	exit(EXIT_SUCCESS);

}
