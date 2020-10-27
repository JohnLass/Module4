/* indexsave1.test.c --- 
 * 
 * 
 * Author: John J. Lass
 * Created: Tue Oct 27 14:07:23 2020 (-0400)
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
	wordcount_t *word1 = (wordcount_t *) malloc(sizeof(wordcount_t));
	wordcount_t *word2 = (wordcount_t *) malloc(sizeof(wordcount_t));
	doc_t *doc1 = (doc_t *) malloc(sizeof(doc_t));
	doc_t *doc2 = (doc_t *) malloc(sizeof(doc_t));
	doc_t *doc3 = (doc_t *) malloc(sizeof(doc_t));
	
	queue_t *document_q1 = qopen();
	queue_t *document_q2 = qopen();

	doc1->doc_id = 1;
	doc1->count = 4;
	doc2->doc_id = 2;
	doc2->count = 3;
	doc3->doc_id = 3;
	doc3->count = 10;

	qput(document_q1,doc1);
	qput(document_q1,doc2);
	qput(document_q2,doc3);
	
	word1->qdoc = document_q1;
	word2->qdoc = document_q2;

	word1->word = "wordex";
	word2->word = "secndone";

	hput(index,word1,word1->word,strlen(word1->word));
	hput(index,word2,word2->word,strlen(word2->word));
	
	
	result = indexsave(index);

	if(result != 0)
		exit(EXIT_FAILURE);

	qclose(document_q1);
	qclose(document_q2);
	hclose(index);
	exit(EXIT_SUCCESS);

}
