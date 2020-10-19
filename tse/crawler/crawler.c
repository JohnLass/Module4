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
	 
	


int main(void){
	char *url = "https://thayer.github.io/engs50/";
	int depth = 0;
	queue_t *qp;
	uint32_t size = 16; 
	hashtable_t *ht;
	webpage_t *w1 = webpage_new(url,depth,NULL);
	webpage_t *HOLD;
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

	fn = printer;
	qapply(qp,fn);

	webpage_delete(w1);
	qclose(qp);
	hclose(ht);

	exit(EXIT_SUCCESS);
}
