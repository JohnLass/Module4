/* crawler.c --- first crawler program
 * 
 * 
 * Author: Robert F. Carangelo III
 * Created: Fri Oct 16 15:51:47 2020 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include "webpage.h"
#include "hash.h"
#include "queue.h"
#include "listfun.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

void printer(void *page){
	webpage_t *wbp = (webpage_t *) page;
	char *qurlp=webpage_getURL(wbp);
	printf("***Internal URL in Queue: %s\n",qurlp);
}

bool web_search(void *page, const void* searchkeyp){
	if((page==NULL) || (searchkeyp==NULL)){
			return false;
	}
	if(strcmp(webpage_getURL((webpage_t *)page),(char *)searchkeyp)!=0){
		return false;
	} else {
		return true;
	}
}
	 
int32_t pagesave(webpage_t *pagep, int id, char *dirname){
	if(pagep != NULL && dirname != NULL){
		FILE *fptr;
		char pathname[50];
		int check1, check2, depth, htmllen;
		char *url, *html;
		
		sprintf(pathname,"../%s/%d",dirname,id);
		check1 = access(pathname, W_OK);
		check2 = access(pathname, F_OK);
		if(check1 != 0 && check2 == 0){
			//mkdir(pathname,0777);
			return -1;
		}
		
		url = webpage_getURL(pagep);
		depth = webpage_getDepth(pagep);
		htmllen = webpage_getHTMLlen(pagep);
		html = webpage_getHTML(pagep);
		
		fptr = fopen(pathname,"w");
		
		fprintf(fptr,"%s\n",url);
		fprintf(fptr,"%d\n",depth);
		fprintf(fptr,"%d\n",htmllen);
		fprintf(fptr,"%s\n",html);
		
		fclose(fptr);
			
		return 0;
	}
	return -1;
}


int main(void){
	char *url = "https://thayer.github.io/engs50/";
	int depth = 0;
	queue_t *qp;
	uint32_t size = 16; 
	hashtable_t *ht;
	webpage_t *w1 = webpage_new(url,depth,NULL);
	webpage_t *HOLD, *w2;
	void (*fn)(void *pagep);
	//bool (*hfunc)(void *page, const void* searchkeyp);
	if(webpage_fetch(w1)){
		printf("Webpage found and fetched\n");
	} else{
		printf("Error getting webpage");
		exit(EXIT_FAILURE);
	}
	// iterating over URLS
	qp=qopen();
	ht=hopen(size);
	//hfunc=web_search;
	int pos=0;
	char *result;
	while((pos=webpage_getNextURL(w1, pos, &result)) > 0){
		if(!(strncmp(url,result,32))){ // confirming that URL is not external

			HOLD = webpage_new(result,depth,NULL);
			///if(hsearch(ht, hfunc, result, strlen(result))==NULL){
				printf("Found Internal URL: %s\n", result);
				//hput(ht, HOLD, result, strlen(result));
				qput(qp,HOLD);
				//}
		}else{
			printf("Found External URL: %s\n", result);
		}
 		free(result);
	}
	
	pagesave(w1,3,"pages");

	w2 = qget(qp);
	pagesave(w2,1,"pages");
	webpage_delete(w2);

	
	
	fn = printer;
	qapply(qp,fn);

	webpage_delete(w1);
	qclose(qp);
	hclose(ht);

	exit(EXIT_SUCCESS);
}
