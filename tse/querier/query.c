/* query.c --- Querier program of TSE
 * 
 * 
 * Author: Tyler I. Neath
 * Created: Sat Oct 31 15:27:45 2020 (-0400)
 * Version: 
 * 
 * Description: reads a query for the user, consults TSE indexer, ranks documents fetcehd by the crawler according to their relevance
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
#include "indexio.c"


#define QLEN 20
#define WLEN 10
int checkString(char *str);
char *NormalizeQword(char *wp);
bool word_search(void *word_countp, const void *searchkeyp);
void count_delete(void *count);
bool doc_search(void *docp, const void *searchkeyp);
void print_queue(void *docp);

typedef struct drank{
	int rank;
	int doc_id;
	char *url;
}drank_t;

int main(void){
	char input[100];
	char query[QLEN][WLEN];
	char *word;
	int flag = 0;
	int min = -1;
	printf("> ");
	hashtable_t *htp = hopen(100);
	wordcount_t *wcp;
	queue_t *qdocp;
	queue_t *results;
	queue_t *backupqp;
	//int id=1;
	doc_t *docp;
	doc_t *curr_doc;
	doc_t *retrieved_doc;
	int *ptr = (int*)1;
	//	drank_t *rp;

	bool (*fn)(void *word_count, const void *searchkeyp);
	void (*fn2)(void *count);
	bool (*fn3)(void *docp, const void *searchkeyp);
	void (*fn4)(void *docp);

	fn = word_search;
	fn2 = count_delete;
	fn3 = doc_search;
	fn4 = print_queue;
	
	if((indexload(htp, "indexnm") != 0)) {
		printf("Error loading index\n");
		exit(EXIT_FAILURE);
	}
	while(fgets(input, 1000, stdin) != NULL){
		//checking for return key
		backupqp = qopen();
		if(strcmp(input,"\n")!=0){
			input[strlen(input)-1] = '\0';
			word = strtok(input, " ");
			int qlen=0;
			while(word!=NULL){
				word[strlen(word)] = '\0';
				strcpy(query[qlen],word);
				word = strtok(NULL," ");
				if(checkString(query[qlen])!=0){
					flag=1;
				}
				qlen++;
			}
			//only proceed if it was a valid search
			if(flag==0) {
				//loop through each word in the query
				for(int i=0; i<qlen; i++) {
					char *holder = NormalizeQword(query[i]);
					if(strlen(holder) >= 3 && strcmp(holder,"and") != 0) {
						//search for the word in the index
						wcp = hsearch(htp,fn,holder,strlen(holder));
						//if the word is in the hashtable
						if(wcp!=NULL) {
							//get its queue of documents 
							qdocp = wcp->qdoc;
							//if it is the first word in the query, set the results queue to this queue
							if(i==0){
								results = (queue_t *) malloc(sizeof(qdocp));
								results = qdocp;
							}
							else {
								//loop through the results queue
								while((curr_doc = qget(results))!=NULL) {
									//if it is in the current word's queue of documents, check if this new word's count is less than the current minimum for this document
									//printf("here3\n");
									if((retrieved_doc = qsearch(qdocp,fn3,curr_doc))!=NULL) {
										//printf("here4\n");
										printf("retrieved doc id: %d, retrieved doc count: %d\n", retrieved_doc->doc_id, retrieved_doc->count);
										if(retrieved_doc->count < curr_doc->count) {
											curr_doc->count = retrieved_doc->count;
										}
										//put it into a new queue
										qput(backupqp,curr_doc);
									}
								}
								//free results and reset it to the backup, 
								free(results);
								results = backupqp;
							}
						}
						else {
							printf("0 documents satisfy this search\n");	
						}	
					}
				}
				qapply(results,fn4);
				/*if(min!= -1) {
					docp->rank = min;
					printf(" -- docid:%d rank: %d\n",docp->doc_id, docp->rank);
				}*/
			}else{
				printf("[Invalid Query]\n");
			}
			printf("> ");
		} else {
			printf("> ");
		qclose(backupqp);
		}
		flag = 0;
		min = -1;
	}
	happly(htp,fn2);	
	hclose(htp);
	exit(EXIT_SUCCESS);
}
	
int checkString(char *str){
	if(str==NULL) {
		return -1;
	}
	int ret = 0;
	int length = strlen(str);
	int alpha;
	for(int i=0; i<(length); i++){
		alpha = isalpha((unsigned char)str[i]);
		if(alpha==0){
			return -1;
		}

	}
	return ret;
}

 char *NormalizeQword(char *wp){
	 int len;
		 if(wp != NULL){
			 len = strlen(wp);
			 char holder[len +1];
			 for(int i=0; i<len; i++){
				 holder[i] = tolower((unsigned char) wp[i]);
			 }
			 holder[len] = '\0';
			 strcpy(wp,holder);
		 } else {
			 wp = NULL;
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

bool doc_search(void *docp, const void *searchkeyp) {
	bool rtn = false;
	doc_t *sdocp;

	if(docp != NULL && searchkeyp != NULL){
		doc_t *dockeyp = (doc_t *)searchkeyp;
		int id = dockeyp->doc_id;
		sdocp = (doc_t*)docp;
		if((sdocp->doc_id)==id){
			printf("true");
			rtn = true;
		}
	}
	return rtn;
}

void count_delete(void *count) {
	if(count != NULL) {
		wordcount_t *wcp = (wordcount_t *) count;
		free(wcp->word);
		qclose(wcp->qdoc);

	}
}

void print_queue(void *docp) {
	if(docp!=NULL){
		doc_t *dp = (doc_t *) docp;
		printf("Doc id: %d, rank: %d\n", dp->doc_id, dp->count);
	}
}