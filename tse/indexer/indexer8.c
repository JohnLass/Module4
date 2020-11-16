/* indexer.c --- Indexer program of TSE WITH MULTI-THREADING
 * 
 * 
 * Author: Robert F. Carangelo III
 * Created: Thu Oct 22 18:12:34 2020 (-0400)
 * Version: 
 * 
 * Description: Builds index that maps a word to the amount of times it shows up on each page that had been crawled
 *              This version does so using 1 pthread
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include "queue.h"
#include "hash.h"
#include "webpage.h"
#include "pageio.c"
//#include "indexio.c"
#include "lindexio.c"
#include "lhash.c"
#include "lhash.h"


#define MAXVAL 50

typedef struct vars_t {
    lhashtable_t *lhtp;
    char *dirname;
} vars_t;


char *NormalizeWord(char *wp);
bool word_search(void *word_countp, const void *searchkeyp);
void count_delete(void *count);
void sumwords(void *count);
doc_t *makedoc(int doc_id, int count);
bool search_doc(void* elementp, const void* keyp);
void *build_index(void *lhtp);


int word_sum = 0;

int main(int argc, char *argv[]) {
    if(argc!=4){
		printf("indexer4 <pagedir> <indexnm> <threadCount>\n");
		exit(EXIT_FAILURE);
	}
	char *dirname = argv[1];
	char *save_name = argv[2];
    int thread_count = atoi(argv[3]);
	char pathname[MAXVAL];
	sprintf(pathname,"../crawler/%s/",dirname);
	
	if((access(pathname, F_OK)) == -1) {
		printf("Invalid directory\n");
		exit(EXIT_FAILURE);
	}

    vars_t *vp = (vars_t *) malloc(sizeof(vars_t));
	lhashtable_t *indexp;
    uint32_t size = 100;

    pthread_t  *id_array; 
    id_array = (pthread_t*)malloc(sizeof(pthread_t)*thread_count);
    
    indexp = lhopen(size);
    vp->lhtp = indexp;
    vp->dirname = dirname;
	
	//initialize thread and have it run bulk of code
    for(int i = 0; i<(thread_count-1); i++) { 
        if(pthread_create(id_array, NULL, build_index, (void *)vp) != 0) {
            printf("Failed making thread\n");
            exit(EXIT_FAILURE);
        }
        id_array++;
    }
    id_array--;
    for(int j = 0; j<(thread_count-1); j++) {
        if(pthread_join(*id_array, NULL) != 0) { 
            printf("Error joining thread\n");
            exit(EXIT_FAILURE);
        }
        id_array--;
    }
    id_array++;
    free(id_array);
	//checking that the calculated sum is equal to the words in the webpage
	//	happly(htp, fn3);
	//printf("Found %d words\n",word_sum);
    
    lindexsave(indexp,save_name);
	
	//free the word stored inside each wordcount_t; they were malloc'd in webpage_GetNextWord and close the hash table
	lhapply(indexp,count_delete);
	lhclose(indexp);
    free(vp);
	//free(docp);
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
			//printf("Wordsum = %d\n Word = %s\n DocID = %d\n Doc_Count = %d\n",word_sum,wcp->word,dp->doc_id,dp->count);
			free(dp);
		}
	}
}

void print_word(void *wordp) {
    if(wordp == NULL) {
        return;
    } 
    wordcount_t *wp = (wordcount_t *) wordp;
    printf("word: %s\n", wp->word);
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
			if (doc->doc_id == *key)
				return(true);
		}
	}
	return(false);
}

/*
 * build_index -- function that is run by each thread. Loops through the documents
 * created by the crawler and builds a locked hashtable of words structs
 *
 * Takes a locked hashtable as an arguement
 * Returns NULL in all cases
 */
void *build_index(void *vp) {
    if(vp == NULL) {
        return NULL;
    }
    vars_t *variables = (vars_t *) vp;
    lhashtable_t *lhp = (lhashtable_t *) variables->lhtp;
    char *wordp = NULL;
	char *lc_wordp = NULL;
	int i;
	webpage_t *wp;
	int pos = 0;
	wordcount_t *foundp;
	queue_t *qp;
	doc_t *docp;
    char *dirname = variables->dirname;

	i = 1;
	while((wp = pageload(i, dirname)) != NULL) {
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
					foundp = lhsearch(lhp,word_search,lc_wordp,strlen(lc_wordp));
					//if its not in the hash table, make a new word count for it, and set it equal to 1, if it is already there, increment it's count and free the word from webpage_getNextWord
					if(foundp == NULL) {
						wordcount_t *wc = (wordcount_t *) malloc(sizeof(wordcount_t));
						wc->word = lc_wordp;
						qp = qopen();
						docp = makedoc(i,1);
						qput(qp, docp);
						wc->qdoc = qp;
						while(lhput(lhp,wc,wc->word,strlen(wc->word)) != 0) {
                        }	
					}
					else {
						qp = foundp->qdoc;
						// if the document is inside the queue, update its value. else create a new document within the queue
						if((docp = qsearch(qp,search_doc,&i)) != NULL) {
							docp->count += 1;
						} else{
							// make new document and place inside queue.
							docp = makedoc(i,1);
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
		i++;
		pos=0;
	}
    return NULL;
}