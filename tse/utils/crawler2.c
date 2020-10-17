/* crawler2.c --- 
 * 
 * 
 * Author: Robert F. Carangelo III
 * Created: Fri Oct 16 18:07:47 2020 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "webpage.h"


int main(void){
	char *url = "https://thayer.github.io/engs50/";
	int depth = 0;
	char *html = NULL;

	webpage_t *w1 = webpage_new(url,depth,html);

	

	exit(EXIT_SUCCESS);
}
