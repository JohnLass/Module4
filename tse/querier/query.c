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
bool isAO(char *wp);
void print_queue(void *docp);
void print_string(void *stringp);
queue_t *answerQuery(char query[QLEN][WLEN],queue_t *results,int qlen, hashtable_t *htp);
queue_t *updateRank(queue_t *results, queue_t *qdocp);
queue_t **parseQuery(char query[QLEN][WLEN], int qlen);
queue_t *emptyQueue(queue_t *curr);
queue_t *qcombine(queue_t *all, queue_t *tmp);
queue_t *qcopy(wordcount_t *wcp);
void sort(queue_t *qp);
bool sort_search(void *docp, const void *searchkeyp);
int cmp(const void *a, const void *b);

int main(void){
	char input[100];
	char query[QLEN][WLEN];
	char *word;
	FILE *fp = stdin;
	//FILE *op =stdout;
	
	
	hashtable_t *htp = hopen(100);
	queue_t *final_result;

	if((indexload(htp, "indexnm") != 0)) {
		printf("Error loading index\n");
		exit(EXIT_FAILURE);
	}
	printf("> ");
	while(fgets(input, 1000, fp) != NULL){
		if(strcmp(input,"\n")==0){						//error checking
			printf("> ");
			continue;
		}
		if(checkString(input) != 0) {					//error checking
			printf("[Invalid Query]\n");
			printf("> ");
			continue;
		}
		input[strlen(input)-1] = '\0';					//normalize string
		word = strtok(input, " ");
		int qlen=0;
		while(word!=NULL){
			strcpy(query[qlen],NormalizeQword(word));
			word = strtok(NULL," ");
			qlen++;
		}	

		final_result = answerQuery(query,final_result,qlen,htp);		//handle the query, bulk of program
		sort(final_result);
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
	FILE *op =stdout;
	if(qlen<1)
		return NULL;
	if(isAO(query[0]) || isAO(query[qlen-1])) {
		fprintf(op,"Invalid Query!\n");
		return NULL;
	}

	wordcount_t *wcp;
	queue_t *all = qopen();
	queue_t *tmp = NULL;

	for(int i=0; i<qlen; i++) {						//loop through all queried words
		if(strcmp("or", query[i]) == 0) {			// if the word is an or
			all = qcombine(all,tmp);
			qclose(tmp);
			tmp = NULL;
			if(isAO(query[i+1])) {
				fprintf(op,"Invalid Query!\n");
				qclose(all);
				return NULL;
			}
		}
		else if(strlen(query[i]) >= 3 && strcmp(query[i],"and") != 0 ) { //if it is a three letter word that is not "and"
			wcp = hsearch(htp,word_search,query[i],strlen(query[i]));
			if (wcp != NULL && tmp == NULL) {	//if word is in index and is first one in an AND sequence
				tmp = qcopy(wcp);
			}
			else if(wcp != NULL && tmp != NULL) {	//if word is in index and is not first one in an AND sequence
				tmp = updateRank(tmp,wcp->qdoc);
			}
			else {									//if word is not in index, stop current AND index
				qclose(tmp);
				tmp = NULL;
				int j = i + 1;
				while(j<qlen) {
					printf("j: %d\n",j);
					if (strcmp(query[j],"or") == 0) {
						break;
					}
					j++;
				}
				i = j;
			}
		}
		if(strcmp("and", query[i]) == 0) {
			if(isAO(query[i+1])) {
				fprintf(op,"Invalid Query!\n");
				return NULL;
			}
		}
	}
	if(tmp != NULL) {
		all = qcombine(all,tmp);
	}
	qclose(tmp);
	return all;
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


/* 
 * parseQuery -- builds an array of queues in which each element of the queue
 * is bound by AND and each queue in the array is bound by OR. Mallocs the array, 
 * opens multiples queues (# of ORs + 1) and mallocs a word for each queried word 
 *
 *	**NOTE this function is from a prior, buggier version, and does not get used
 *
 */  
queue_t **parseQuery(char query[QLEN][WLEN], int qlen) {
	if(qlen == 0) {
		return NULL;
	}
	if((strcmp("and", query[0])) == 0 || strlen(query[0]) < 3) {
		printf("Invalid Query!\n");
		return NULL;
	}
	if(strcmp("and", query[qlen-1]) == 0 || strcmp("or", query[qlen-1]) == 0 ) {
		printf("Invalid Query!\n");
		return NULL;		
	}
	queue_t **andq = (void*) malloc(sizeof(queue_t *)*qlen);
	int index = 0;

	queue_t *curr = qopen();
	char *holder = (char *) malloc(sizeof(char)*strlen(query[0]));
	qput(curr,query[0]);
	holder = NULL;
	
	for(int i = 1; i<qlen ; i++){
		if(strlen(query[i]) >= 3 && (strcmp(query[i],"and")) != 0){
			holder = (char *) malloc(sizeof(char)*strlen(query[i]));
			strcpy(holder,query[i]);
			qput(curr,holder);
			holder = NULL;
		}
		if(strcmp(query[i],"or") == 0){
			andq[index] = emptyQueue(curr);
			index++;
			curr = qopen();
			printf("index: %d\n", index);
		}
	}
	
	andq[index] = emptyQueue(curr);
	index++;

	while(index<qlen){
		andq[index] = NULL;
		index++;
		printf("index:%d\n",index);
	}
	
	for(int j = 0; j<qlen; j++){
		printf("j: %d index: %d", j, index);
		qapply(andq[j],print_string);
		printf("\n\n");
	}

	return andq;
}	
// qapply funciton for a queue of strings
void print_string(void *stringp) {
	if(stringp != NULL) {
		char *sp = (char *) stringp;
		printf("%s ",sp);
	}
}

/*
 * emptyQueue -- empties a queue into a returned queue, 
 * closes the input queue and opens the returned one
 *
 *	**NOTE this is from a prior, buggier version, and does not get used
 *
 */
queue_t *emptyQueue(queue_t *curr) {
	if(curr==NULL) {
		return NULL;
	}
	queue_t *rtn = qopen();
	char *holder;
	while((holder = qget(curr)) != NULL) {
		qput(rtn,holder);
	}
	qclose(curr);
	return rtn;
}

/*
 * qcombine -- combines two queues of documents. When the two queues repeat an element
 * it adds their ranks.
 * Opens: one queue that is returned
 * Closes: one queues passed to it
 */
queue_t *qcombine(queue_t *all, queue_t *tmp) {
	if(tmp == NULL) {
		return NULL;
	}
	doc_t *holder;
	doc_t *sameD;
	queue_t *newq = qopen();
	while((holder = qget(tmp)) != NULL) {
		if((sameD = qremove(all,doc_search,holder)) != NULL) {
			holder->count = holder->count + sameD->count;
			free(sameD);
		}
		qput(newq, holder);
	}
	while((holder = qget(all)) != NULL) {
		qput(newq, holder);
	}
	qclose(all);
	return newq;
}


/*
 * qcopy -- makes a copy of the queue of documents stored in the input webpage. 
 * returns the copied queue. Opens a queue. Allocates space for doc_t in the new queue.
 */
queue_t *qcopy(wordcount_t *wcp) {
	if(wcp == NULL){
		return NULL;
	}

	queue_t *indexq = wcp->qdoc;
	queue_t *rtnq = qopen();
	queue_t *backupqp = qopen();
	doc_t *curr_doc;
	doc_t *copied_doc;
	while((curr_doc = qget(indexq)) != NULL) {
		copied_doc = (doc_t *) malloc(sizeof(doc_t));
		copied_doc->doc_id = curr_doc->doc_id;
		copied_doc->count = curr_doc->count;
		qput(rtnq,copied_doc);
		qput(backupqp,curr_doc);
	}
	qclose(indexq);
	wcp->qdoc = backupqp;
	return rtnq;
}

bool isAO(char *wp) {
	if(wp == NULL) {
		return false;
	}
	if(strcmp(wp,"or") == 0) {
		return true;
	}
	if(strcmp(wp,"and") == 0) {
		return true;
	}
	return false;
}

void sort(queue_t *qp) {
	if(qp == NULL){
		return;
	}
	queue_t *backupq = qopen();
	int array[500];
	int i = 0;
	doc_t *holder;

	while((holder = qget(qp)) != NULL){
		array[i] = holder->count;
		qput(backupq,holder);
		i++;
	}
	if(i>0) {
		qsort(array,i,sizeof(int),cmp);
		for(int j = 0; j<i; j++) {
			int rank = array[j];
			holder = qremove(backupq,sort_search,&rank);
			qput(qp,holder);
		}
	}
	else {
		printf("no result\n");
	}

	qclose(backupq);
	
}

bool sort_search(void *docp, const void *searchkeyp) {

	if(docp != NULL && searchkeyp != NULL) {
		doc_t *doc = (doc_t*) docp;
		return doc->count== * (int*)searchkeyp;
	}
	return false;
}

int cmp(const void *a, const void *b) {
	return ( *(int*)b - *(int*)a );
}
