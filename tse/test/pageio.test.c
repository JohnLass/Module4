/* pageio.test.c --- Tests pageio.c module
 * 
 * 
 * Author: Robert F. Carangelo III
 * Created: Wed Oct 21 14:58:28 2020 (-0400)
 * Version: 
 * 
 * Description: Makes a webpage, fetches its html, saves it to a file, loads a new webpage from the file and compares the two
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <pageio.h>


int main(void) {
	webpage_t *wp1, *wp2;
	char *url = "https://thayer.github.io/engs50/";
	char *dirname = "pages";
	int depth = 1;
	int id = 1;
	char *holder;

	wp1 = webpage_new(url, depth, NULL);

	if(!(webpage_fetch(wp1))){
		webpage_delete(wp1);
		exit(EXIT_FAILURE);
	}
	
	pagesave(wp1, id, dirname);

	wp2 = pageload(id, dirname);

	printf("%s\n", webpage_getHTML(wp1));
	printf("*******************************************************\n");
  printf("%s\n", webpage_getHTML(wp2));

	holder = webpage_getURL(wp1);
	if(strcmp(webpage_getURL(wp1),webpage_getURL(wp2)) != 0){
		printf("Fail1\n");
		webpage_delete(wp1);
		webpage_delete(wp2);
		exit(EXIT_FAILURE);
	}
	if(webpage_getDepth(wp1) != webpage_getDepth(wp2)){
		printf("Fail2\n");
		webpage_delete(wp1);
		webpage_delete(wp2);
		exit(EXIT_FAILURE);
	}
	if(strcmp(webpage_getHTML(wp1),webpage_getHTML(wp2)) != 0){
		printf("Fail3\n");
		webpage_delete(wp1);
		webpage_delete(wp2);
		exit(EXIT_FAILURE);
	}
	
	if (wp1 != NULL)
		webpage_delete(wp1);
	if (wp2 != NULL)
		webpage_delete(wp2);
	
	exit(EXIT_SUCCESS);
}
	
