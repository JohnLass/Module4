/* indexer.c --- Indexer program of TSE
 * 
 * 
 * Author: Robert F. Carangelo III
 * Created: Thu Oct 22 18:12:34 2020 (-0400)
 * Version: 
 * 
 * Description: Builds index that maps a word to the amount of times it shows up on each page that had been crawled
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "hash.h"
#include "webpage.h"
#include "pageio.c"

int main(void) {
	char *dirname = NULL;
	char **word = NULL;
	int id;
	webpage_t *wp;
	FILE *fp;
	int pos = 0;

	dirname = "pages";
	word = &dirname;
	id = 1;

	if((fp = fopen("word_index.txt","w")) != NULL) {
		wp = pageload(id, dirname);

		while(pos >= 0) {
			pos = webpage_getNextWord(wp,pos,word);
			if(pos >= 0) {
				fprintf(fp,"%s\n", *word);
				free(*word);
			}
		}
		
		webpage_delete(wp);
		fclose(fp);
	}
	exit(EXIT_SUCCESS);
}
