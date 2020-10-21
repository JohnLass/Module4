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
#include "pageio.h"
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
	int depth = webpage_getDepth(wbp);
	printf("***Internal URL in Queue: %s\n",qurlp);
	printf("It's html is %d levels deep\n",depth);
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

int main(int argc, char *argv[]){
	//check number of args
	if(argc != 4){
		printf("crawler <seedurl> <pagedir> <maxdepth>\n");
		exit(EXIT_FAILURE);
	}
	
	//convert args into usable variables
	char *url = argv[1]; //"https://thayer.github.io/engs50/";
	char *pagedir = argv[2];
	int max_depth = atoi(argv[3]);

	//check arguments
	if(url == NULL || pagedir == NULL || max_depth <0){
		printf("invalid args\n");
		exit(EXIT_FAILURE);
	}
	//check that pagedir exists
	struct stat stats;
	if(stat(pagedir, &stats) != 0 || !S_ISDIR(stats.st_mode)){
		printf("invalid directory\n");
		exit(EXIT_FAILURE);	
	}
	char *seed_url = (char *) calloc(strlen(url) + 1, sizeof(char));
 //char *seed_url = (char *) malloc(sizeof(char)*strlen(url));
	queue_t *qp;
	uint32_t size = 100;
	int depth = 0;
	hashtable_t *ht;
	webpage_t *w1 = webpage_new(url,depth,NULL);
	webpage_t *HOLD, *curr;
	void (*fn)(void *pagep);
	bool (*hfunc)(void *page, const void* searchkeyp);
	char *urlp2;
	//fetches the HTML from the seed url and puts it in it's url variable, checks if successful
	//opens up queue of webpages, hashtable of urls (strings) and puts the seed in first
	strcpy(seed_url,url);
	qp=qopen();
	ht=hopen(size);
	qput(qp,w1);
	hput(ht, seed_url,seed_url, strlen(seed_url));

 
	//declares hash search fn
	hfunc=web_search;
	fn = printer;
	char *urlp = NULL;
	int loop_count = 0;
 	while((curr = qget(qp))!=NULL){
		
		loop_count++;
		printf("loopcount: %d\n",loop_count);
		depth = webpage_getDepth(curr);
		
		
		
		
		if(depth<=max_depth){
			printf("depth : %d\n", depth);
			if(webpage_fetch(curr)){
				//	printf("webpage found again!\n");
				pagesave(curr,loop_count,pagedir);

				if(depth<max_depth){
					int pos=0;
				
				
					// iterating over URLS in curr's HTML
					while((pos = webpage_getNextURL(curr, pos, &urlp)) > 0){
						// confirming that URL is not external	
						if(IsInternalURL(urlp)){ 
							//checks if the URL is already in the hashtable, if not add URL to hashtable, make webpage struct with url = curl and put it in the queue
							if(hsearch(ht, hfunc, urlp, strlen(urlp))==NULL){
								hput(ht, urlp, urlp, strlen(urlp));
								HOLD = webpage_new(urlp, depth + 1,NULL);
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
				}
			
			}
		}
		webpage_delete(curr);
		
	}
	
	qapply(qp,fn);
	
	qclose(qp);
	hclose(ht);
	
	exit(EXIT_SUCCESS);
}
