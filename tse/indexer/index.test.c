/* index.test.c --- 
 * 
 * 
 * Author: John J. Lass
 * Created: Wed Oct 28 21:43:38 2020 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "indexio.h"
#include "queue.h"
#include "hash.h"


typedef struct wordcount {
	char *word;
	queue_t *qdoc;
} wordcount_t;

typedef struct doc {
	int doc_id;
	int count;
} doc_t;

int main(void) {
	
	int result;
	hashtable_t *index = hopen(100);
 	hashtable_t *index2 = hopen(100);
	wordcount_t *word1 = (wordcount_t *) malloc(sizeof(wordcount_t));
	wordcount_t *word2 = (wordcount_t *) malloc(sizeof(wordcount_t));
	wordcount_t *word3 = (wordcount_t *) malloc(sizeof(wordcount_t));
	doc_t *doc1 = (doc_t *) malloc(sizeof(doc_t));
	doc_t *doc2 = (doc_t *) malloc(sizeof(doc_t));
	doc_t *doc3 = (doc_t *) malloc(sizeof(doc_t));
	doc_t *doc4 = (doc_t *) malloc(sizeof(doc_t));
	
	queue_t *document_q1 = qopen();
	queue_t *document_q2 = qopen();
	queue_t *document_q3 = qopen();

	doc1->doc_id = 1;
	doc1->count = 4;
	doc2->doc_id = 2;
	doc2->count = 3;
	doc3->doc_id = 6;
	doc3->count = 10;
	doc4->doc_id = 5;
	doc4->count = 9;
	

	qput(document_q1,doc1);
	qput(document_q1,doc2);
	qput(document_q2,doc3);
	qput(document_q3,doc4);
	
	word1->qdoc = document_q1;
	word2->qdoc = document_q2;
	word3->qdoc = document_q3;

	word1->word = "wordex";
	word2->word = "secndone";
	word3->word = "freguj";

	hput(index,word1,word1->word,strlen(word1->word));
	hput(index,word2,word2->word,strlen(word2->word));
	hput(index,word3,word3->word,strlen(word3->word));
	
	
	if((result = indexsave(index, "test_test"))!=0) {
		printf("Unable to save test_test\n");
		exit(EXIT_FAILURE);
	}
	if((result = indexload(index2, "test_test"))!=0) {
		printf("Unable to load test_test\n");
		exit(EXIT_FAILURE);
	}
	if((result = indexsave(index2, "test_test2"))!=0) {
		printf("Unable to save test_test2\n");	
		exit(EXIT_FAILURE);
	}
	qclose(document_q1);
	qclose(document_q2);
	qclose(document_q3);
	hclose(index);
	hclose(index2);
	printf("Successful test! From VS code!\n");
	exit(EXIT_SUCCESS);

}
