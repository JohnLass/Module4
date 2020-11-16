/* lindexio.c --- 
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
#include "lhash.h"
#include "lqueue.h"


typedef struct wordcount {
	char *word;
	queue_t *qdoc;
	bool locked;
} wordcount_t;

typedef struct doc {
	int doc_id;
	int count;
	int rank;
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
int lindexsave(lhashtable_t *lhtp, char *filename) {
	if(lhtp == NULL || filename == NULL)
		return -1;

	void (*fn)(void *word);
	fn = print_to_file;

	if((fp = fopen(filename,"w")) != NULL) {
		lhapply(lhtp,fn);
		fclose(fp);
	}
	return 0;
}

int lindexload(lhashtable_t *lhtp, char *fname){

	FILE *fnamep;
	char indwp[256];
	int doc_id,id_count;
	queue_t *qp;
	wordcount_t *varp;
	doc_t *dp;
	char *wp;
	int len;

	if(!(fnamep=fopen(fname,"r"))){
		printf("Cannot read file");
		return -1;
	}
	while(fscanf(fnamep,"%s",indwp)!=EOF){
		len = strlen(indwp);
		if((wp = malloc(len+1))==NULL)
			return -1;
		if((varp = (wordcount_t*)malloc(sizeof(wordcount_t)))==NULL)
			return -1;
		strcpy(wp,indwp);
		qp=qopen();
		while(fscanf(fnamep,"%d%d",&doc_id,&id_count)==2){
			if((dp = malloc(sizeof(doc_t)))==NULL)
				return -1;
			dp->doc_id=doc_id;
			dp->count=id_count;
			qput(qp,dp);
		}
		varp->qdoc=qp;
		varp->word=wp;
		lhput(lhtp,varp,wp,len);
	}
	fclose(fnamep);
	return 0;
}
				
