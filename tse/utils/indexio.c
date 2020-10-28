/* indexio.c --- 
 * 
 * 
 * Author: John J. Lass
 * Created: Tue Oct 27 13:53:25 2020 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "queue.h"
#include "hash.h"
#include "webpage.h"



typedef struct wordcount {
	char *word;
	queue_t *qdoc;
} wordcount_t;

typedef struct doc {
	int doc_id;
	int count;
} doc_t;

//global variable so that happly function "print_to_file can access the same file repeteadly without it being passed as an argument
FILE *fp;

/*function to be passed to happly, that prints each word and its data to a new line in the file*/
void print_to_file(void *word) {
	if(word!=NULL) {
		wordcount_t *wordp = (wordcount_t *)word;
		doc_t *dp;

		fprintf(fp,"%s",wordp->word);
		
		if(wordp->qdoc != NULL) {
			while((dp = (doc_t*) qget(wordp->qdoc)) != NULL) {
				fprintf(fp," %d %d",dp->doc_id, dp->count);
				free(dp);
			}
		}
		fprintf(fp,"\n");
	}
}

/*
 * indexsave -- takes an index as an arguement and prints it to the file "indexnm" in the following form:
 *<word> <doc1id> <doc1count> <doc2id> <doc2count> ...
 *
 */
int indexsave(hashtable_t *htp){
	if(htp == NULL)
		return -1;

	void (*fn)(void *word);
	fn = print_to_file;

	if((fp = fopen("indexnm_test","w")) != NULL) {
		happly(htp,fn);
		fclose(fp);
	}
	return 0;
}

int indexload(char *fname){

	FILE *fnamep;
	char holder[30];
	void (*fn)(void *word);
	hashtable_t *htp;
	uint32_t i,n=0;
	
	if(!(fnamep=fopen(fname,"r"))){
		printf("Cannot read file");
		return -1;
	}

	if((fp = fopen("indexnm","w")) != NULL){
		while((fgets(holder,30,fnamep))){
			n++;
		}
	}

	htp = hopen(n);

	for(i=0;i<n;i++){
		hput(htp
	printf("Exited loop");
	fclose(fp);
	fclose(fnamep);
	return 0;

}
				
