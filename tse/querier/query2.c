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
queue_t *answerQuery(char query[QLEN][WLEN],queue_t *results,int qlen, hashtable_t *htp);
queue_t *updateRank(queue_t *results, queue_t *qdocp);

int main(void){
	char input[100];
	char query[QLEN][WLEN];
	char *word;
	
	hashtable_t *htp = hopen(100);
	queue_t *final_result;

	if((indexload(htp, "indexnm") != 0)) {
		printf("Error loading index\n");
		exit(EXIT_FAILURE);
	}
	printf("> ");
	while(fgets(input, 1000, stdin) != NULL){
		final_result = qopen();
		//checking for return key
		if(strcmp(input,"\n")==0){
			printf("> ");
			continue;
		}
		if(checkString(input) != 0) {
			printf("[Invalid Query]\n");
			printf("> ");
			continue;
		}
		input[strlen(input)-1] = '\0';
		word = strtok(input, " ");
		int qlen=0;
		while(word!=NULL){
			strcpy(query[qlen],NormalizeQword(word));
			word = strtok(NULL," ");
			qlen++;
		}	
		//loop through each word in the query
		final_result = answerQuery(query,final_result,qlen,htp);
		
		qapply(final_result,print_queue);
		qclose(final_result);
		printf("> ");
	}
	happly(htp,count_delete);	
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
		if(alpha==0 && isspace(str[i]) == 0){
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
		FILE *fp;
		int id = dp->doc_id;
		char filename[50];
		char url[256];
		sprintf(filename, "../crawler/pages/%d", id);

		if((fp = fopen(filename,"r")) == NULL) 		
			return;
		
		if((fgets(url,256,fp)) == NULL)
			return;
		fclose(fp);

		printf("rank: %d: doc: %d: url: %s", dp->count, dp->doc_id, url);
	}
}

/*
 * answerQuery -- Loops through the queried words and builds a queue of documents 
 * named results that each word shows up in and contains the rank of each
 */
queue_t *answerQuery(char query[QLEN][WLEN],queue_t *results,int qlen, hashtable_t *htp) {
	queue_t *backupqp = qopen();
	queue_t *qdocp;
	wordcount_t *wcp;
	doc_t *copied_doc;
	doc_t *curr_doc;

	wcp = hsearch(htp,word_search,query[0],strlen(query[0]));
	if(wcp == NULL){
		printf("Word 1 not found\n");
		return NULL;
	}
		
	queue_t *firstq = wcp->qdoc;
	while((curr_doc = qget(firstq)) != NULL) {
		copied_doc = (doc_t *) malloc(sizeof(doc_t));
		copied_doc->doc_id = curr_doc->doc_id;
		copied_doc->count = curr_doc->count;
		qput(results,copied_doc);
		qput(backupqp,curr_doc);
	}

	qclose(firstq);
	wcp->qdoc = backupqp;
	for(int i=1; i<qlen; i++) {
		if(strlen(query[i]) >= 3 && strcmp(query[i],"and") != 0) {	
			wcp = hsearch(htp,word_search,query[i],strlen(query[i]));
			if(wcp!=NULL) {		
				qdocp = wcp->qdoc;
				results = updateRank(results,qdocp);
			}
			else {
				printf("Word %d not found\n",(i+1));
				return NULL;
			}
		}				
	}
	return results;
}

/*
 * updateRank -- loops through the "results" queue and takes out documents if they don't contain
 * the subsequent queried words and also updates rank
 */
queue_t *updateRank(queue_t *results, queue_t *qdocp) {
	if(results == NULL || qdocp == NULL ) {
		printf("Passed null pointer!\n");
		return NULL;
	}

	doc_t *curr_doc;
	doc_t *retrieved_doc;
	queue_t *backupqp = qopen();

	while((curr_doc = qget(results))!=NULL) {
		if((retrieved_doc = qsearch(qdocp,doc_search,curr_doc))!=NULL) {
			if(retrieved_doc->count < curr_doc->count) {
				curr_doc->count = retrieved_doc->count;
			}
			qput(backupqp,curr_doc);
		} 
		else {
			free(curr_doc);
		}	
	}

	free(results);
	results = backupqp;
	return results;
}

	


