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
	doc_t *docp;

	void (*fn2)(void *count);
	bool (*fn)(void *word_count, const void *searchkeyp);
	
	fn = word_search;
	fn2 = count_delete;

	if((indexload(htp, "indexnm") != 0)) {
		printf("Error loading index\n");
		exit(EXIT_FAILURE);
	}
	while(fgets(input, 1000, stdin) != NULL){
		//checking for return key
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
			//checking for invalid characters
			if(flag==0){
				for(int i=0; i<qlen; i++){
					char *holder = NormalizeQword(query[i]);
					if(strlen(holder) >= 3 && strcmp(holder,"and") != 0){
						wcp = hsearch(htp,fn,holder,strlen(holder));
						if(wcp==NULL) {
							printf("%s:0 ",holder);
							min = 0;
						}
						else {
							qdocp = wcp->qdoc;
							docp = qget(qdocp);
							printf("%s:%d ",holder,docp->count);
							if(min == -1 || min>docp->count)
								min = docp->count;
							qput(qdocp,docp);
						}	
					}
				}
				if(min!= -1)
					printf(" -- %d\n",min);
			}else{
				printf("[Invalid Query]\n");
			}
			printf("> ");
		} else {
			printf("> ");
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