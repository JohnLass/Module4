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
	queue_t *qdoc;
} wordcount_t;

typedef struct doc {
	int doc_id;
	int count;
} doc_t;

char *NormalizeWord(char *wp);
bool word_search(void *word_countp, const void *searchkeyp);
void count_delete(void *count);
void sumwords(void *count);
doc_t *makedoc(int doc_id, int count);
bool search_doc(void* elementp, const void* keyp);

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
	bool (*fn4)(void* elementp, const void* keyp);

	//busiwork
	// FILE *fp2;

	//initialize variables, open hash table, declare function pointers
	dirname = "pages";
	id = 1;
	htp = hopen(size);
	fn = word_search;
	fn2 = count_delete;
	fn3 = sumwords;
	fn4 = search_doc;

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
						queue_t *qp = qopen();
						doc_t *docp = makedoc(id,1);
						qput(qp, docp);
						wc->qdoc = qp;

						hput(htp,wc,wc->word,strlen(wc->word));
						fprintf(fp,"%s\n", lc_wordp);
						break;
					}
					else {
						queue_t *qp = foundp->qdoc;
						// if the document is inside the queue, update its value. else create a new document within the queue
						doc_t* docp;
						if((docp = qsearch(qp,fn4,&id)) != NULL){
							docp->count += 1;
						} else{
							// make new document and place inside queue.
							docp = makedoc(id,1);
							qput(qp,docp);
						}
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
 * function also frees the queue of documents
 */
void count_delete(void *count) {
	if(count != NULL) {
		printf("Is deleting\n");
		wordcount_t *wcp = (wordcount_t *) count;
		free(wcp->word);
		qclose(wcp->qdoc);
	}
}

/* sumwords -- calculates the sum of all words within the  */
void sumwords(void *count){
	if(count != NULL){
		doc_t *dp;
		wordcount_t *wcp = (wordcount_t *) count;
		while((dp = (doc_t*) qget(wcp->qdoc))!=NULL){
			word_sum += dp->count;
		}
	}
}

/* makedoc -- makes document structure that is to be put inside queue. returns a pointer to said strcuture */
doc_t *makedoc(int doc_id, int count){
	doc_t *dp = NULL;
	if((dp = (doc_t*)malloc(sizeof(doc_t)))!=NULL){
		dp->doc_id = doc_id;
		dp->count = count;
	}
	return dp;
}

/* search_doc -- searches function for the queue of documents. if found, returns true. false if otherwise */
bool search_doc(void* elementp, const void* keyp){
	if(elementp!=NULL){
		doc_t *doc = (doc_t*) elementp;
		if(keyp != NULL){
			int *key = (int*) keyp;
			return(doc->doc_id == *key);
		}
	}
	return(false);
}
