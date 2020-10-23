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
#include <ctype.h>
#include "queue.h"
#include "hash.h"
#include "webpage.h"
#include "pageio.c"


typedef struct wordcount {
	char *word;
	int count;
} wordcount_t;

char *NormalizeWord(char *wp);

int main(void) {
	char *dirname = NULL;
	char *wordp = NULL;
	char *lc_wordp = NULL;
	int id;
	webpage_t *wp;
	FILE *fp;
	int pos = 0;
	char *holderp = NULL;

	
	dirname = "pages";
	id = 1;

	if((fp = fopen("word_index.txt","w")) != NULL) {
		wp = pageload(id, dirname);

		while(pos >= 0) {
			pos = webpage_getNextWord(wp,pos,&wordp);
			if(pos >= 0) {
				lc_wordp = NormalizeWord(wordp);
				if(lc_wordp != NULL)
					fprintf(fp,"%s\n", lc_wordp);
				free(wordp);
			}
		}
		
		webpage_delete(wp);
		fclose(fp);
	}
	exit(EXIT_SUCCESS);
}


char *NormalizeWord(char *wp) {
	int len;
	
	
	if(wp != NULL) {
		len = strlen(wp);
		if(len > 2) {
			char holder[len +1];
			for(int i = 0; i<len; i++) {
				if(isalpha(wp[i])) {
					holder[i] = tolower((unsigned char) wp[i]);
				}
			}
			holder[len] = '\0';
			//wp = holder;
			strcpy(wp,holder);
		}
		else {
			wp = NULL;
		}
	}
	return wp;
}
