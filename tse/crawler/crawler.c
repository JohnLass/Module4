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

int main(void){
	char *url = "https://thayer.github.io/engs50/";
	int depth = 0;
	
	webpage_t *w1 = webpage_new(url,depth,NULL);

	if(webpage_fetch(w1)){
		printf("Webpage found and fetched\n");
		printf("%s\n",webpage_getHTML(w1));		
	} else{
		printf("Error getting webpage");
		exit(EXIT_FAILURE);
	}

	
	// iterating over URLS
	int pos=0;
	char *result;
	while((pos=webpage_getNextURL(w1, pos, &result)) > 0){
		printf("Found url: %s\n", result);
		free(result);
	}
	
		


	webpage_delete(w1);
	exit(EXIT_SUCCESS);

}
