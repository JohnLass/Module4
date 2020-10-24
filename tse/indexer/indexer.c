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
bool word_search(void *word_countp, const void *searchkeyp);
void count_delete(void *count);
void count_sum(void *count);

int word_sum = 0;

int main(void) {
	char *dirname = NULL;
	char *wordp = NULL;
	char *lc_wordp = NULL;
	int id;
	webpage_t *wp;
	FILE *fp;
	int pos = 0;
	hashtable_t *htp;
	uint32_t size = 100;
	wordcount_t *foundp;
	bool (*fn)(void *word_count, const void *searchkeyp);
	void (*fn2)(void *count);
	void (*fn3)(void *count);

	//busiwork
	// FILE *fp2;

	//initialize variables, open hash table, declare function pointers
	dirname = "pages";
	id = 1;
	htp = hopen(size);
	fn = word_search;
	fn2 = count_delete;
	fn3 = count_sum;

	//open the document to be written to, then load the appropriate webpage
	if((fp = fopen("word_index2.txt","w")) != NULL) {
		wp = pageload(id, dirname);
		//loop through the words in the html from that webpage
		while(pos >= 0) {
			pos = webpage_getNextWord(wp,pos,&wordp);
			//if we aren't at the last word
			if(pos >= 0) {
				//make the word lowercase
				lc_wordp = NormalizeWord(wordp);
				//if the word was at least 3 chars long
				if(lc_wordp != NULL) {
					//search for the word in the hash table
					foundp = hsearch(htp,fn,lc_wordp,strlen(lc_wordp));
					//if its not in the hash table, make a new word count for it, and set it equal to 1, if it is already there, increment it's count and free the word from webpage_getNextWord
					if(foundp == NULL) {
						wordcount_t *wc = (wordcount_t *) malloc(sizeof(wordcount_t));
						wc->word = lc_wordp;
						wc->count = 1;
						hput(htp,wc,wc->word,strlen(wc->word));
						fprintf(fp,"%s\n", lc_wordp);
					}
					else {
						foundp->count = foundp->count + 1;
						free(lc_wordp);
					}
				}
				else {
					//if the word wasn't three characters long, still free the word from webpage_getNextWord
					free(wordp);
				}
			}
		}
		//close the webpage and the file
		webpage_delete(wp);
		fclose(fp);	
	}
	//shows that it worked
	foundp = hsearch(htp,fn,"the",3);
	printf("Found %s %d times\n",foundp->word,foundp->count);

	//checking that the calculated sum is equal to the words in the webpage
	happly(htp, fn3);
	printf("Found %d words\n",word_sum);


	//free the word stored inside each wordcount_t; they were malloc'd in webpage_GetNextWord and close the hash table
	happly(htp,fn2);
	hclose(htp);
	exit(EXIT_SUCCESS);
}

/* NormalizeWord -- changes all characters in a word to 0
 * wp must be at least 3 characters
 * returns a pointer to the lowercase word, or NULL if input doesn't fit criterea
 */
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

/* word_search -- hashtable search function to see if a word is in the hash table, does so by comparing the to arguments
 * returns true if it is in the table -- if the two arguments are the same
 * returns false if it is not in the table -- if the two arguments are different
 */
bool word_search(void *word_countp, const void *searchkeyp) {
	bool rtn = false;
	char *w1;
	wordcount_t *wcp;
	
	if(word_countp != NULL && searchkeyp != NULL) {
		wcp = (wordcount_t *) word_countp;
		w1 = (char *) searchkeyp;
		if(strcmp(wcp->word,w1) == 0)
			rtn = true;
	}
	return rtn;
}

/* count_delete -- hashtable apply function that frees the words that are stored in wordcount_t structures in the hash table
 * these words were malloc'd in webpage_GetNextWord
 */
void count_delete(void *count) {
	if(count != NULL) {
		wordcount_t *wcp = (wordcount_t *) count;
		free(wcp->word);
	}
}

/* count_sum -- calculates the sum of all words within the hashtable */
void count_sum(void *count){
	if(count != NULL){
		wordcount_t *wcp = (wordcount_t *) count;
		word_sum += wcp->count;
	}
}
