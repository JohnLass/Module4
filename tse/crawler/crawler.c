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
#include "queue.h"
#include "hash.h"
#include "webpage.h"


int main(void){
	char *url = "http://thayer.github.io/engs50/";
	int depth = 0;
	char *html =NULL;

	webpage_t *w1 = webpage_new(url,depth,html);;
	int w2;


	w2 = webpage_getDepth(w1);
	
	printf("%d\n", w2);
	
	printf("Hello -- Crawlerp--!\n");

	webpage_delete(w1);
	
	exit(EXIT_SUCCESS);

}
