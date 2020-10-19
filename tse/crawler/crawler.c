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
	int htmllen = webpage_getHTMLlen(wbp);
	printf("***Internal URL in Queue: %s\n",qurlp);
	printf("It's html is %d chars long\n",htmllen);
}

bool web_search(void *urlp, const void* searchkeyp){
	if((urlp==NULL) || (searchkeyp==NULL)){
			return false;
	}
	if(strcmp((char *)urlp,(char *)searchkeyp)!=0){
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
	bool (*hfunc)(void *page, const void* searchkeyp);
	
	//fetches the HTML from the seed url and puts it in it's url variable, checks if successful
	if(webpage_fetch(w1)){
		printf("Webpage found and fetched\n");
	} else{
		printf("Error getting webpage");
		exit(EXIT_FAILURE);
	}
	//opens up queue of webpages, hashtable of urls (strings) and declares the hashtable search function
	qp=qopen();
	ht=hopen(size);
	hfunc=web_search;

	// iterating over URLS in seed's HTML
	int pos=0;
	char *urlp;
	while((pos=webpage_getNextURL(w1, pos, &urlp)) > 0){
		// confirming that URL is not external	
		if(!(strncmp(url,urlp,32))){ 
			//checks if the URL is already in the hashtable, if not add URL to hashtable, make webpage struct with url = curl and put it in the queue
			if(hsearch(ht, hfunc, urlp, strlen(urlp))==NULL){
				hput(ht, urlp, urlp, strlen(urlp));
				HOLD = webpage_new(urlp,depth,NULL);
				printf("Found Internal URL: %s\n", urlp);
				qput(qp,HOLD);				
			}
			else {
				printf("URL already in hashtable:%s\n", urlp);
				free(urlp);
			}
		}
		else{
			printf("Found External URL: %s\n", urlp);		
			free(urlp);
		}
		
	}
	
	//saves the first page and deletes w1
	pagesave(w1,0,"pages");
	webpage_delete(w1);

	//Prints everything in the queue
	fn = printer;
	qapply(qp,fn);

	//takes out the first item, gets second item in queue (this is to test fetching another html)
	w2 = qget(qp);
	webpage_delete(w2);
	w2 = qget(qp);

	//fetches the second items html
	if(webpage_fetch(w2)){
		printf("Webpage found and fetched\n");
	} else{
		printf("Error getting webpage");
		exit(EXIT_FAILURE);
	}

	//saves second item, HTML should be non-NULL
	pagesave(w2,15,"pages");
	webpage_delete(w2);

	//loop through rest of queue, save each one in a file that is numbered, delete each webpage as you get it
	HOLD = qget(qp);
	int counter = 1;
	while(HOLD!=NULL){
		pagesave(HOLD,counter,"pages");
		webpage_delete(HOLD);
		HOLD = qget(qp);
		counter++;
	}

	//close queue and hashtable
	qclose(qp);
	hclose(ht);
	
	exit(EXIT_SUCCESS);
}
