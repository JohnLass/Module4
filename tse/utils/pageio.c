/* pageio.c --- Module that implements pageio.h 
 * 
 * 
 * Author: Robert F. Carangelo III
 * Created: Wed Oct 21 14:00:25 2020 (-0400)
 * Version: 
 * 
 * Description: Saves webpages from the crawl and loads webpage files into webpage_t structures
 * 
 */

#include "pageio.h"
#include "webpage.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXVAL 50
#define MAXURL 250
/* 
 * pagesave -- save the page in filename id in dirctory dirnm
 * returns: 0 for success; -1 for failure
 * Format of saved file is:
 * <url>
 * <depth>
 * <html-length>
 * <html>
 */
int32_t pagesave(webpage_t *pagep, int id, char *dirnm) {

	if(pagep != NULL && dirnm != NULL){
		FILE *fptr;
		char pathname[MAXVAL];
		int check1, check2, depth, htmllen;
		char *url, *html;
		
		sprintf(pathname,"./%s/%d",dirnm,id);
		check1 = access(pathname, W_OK);
		check2 = access(pathname, F_OK);
		if(check1 != 0 && check2 == 0) //checks if the file exists but isn't writeable
			return -1;
		
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


/*
 * pageload -- loads the numbered filename <id> in directory <dirnm> into a new webpage
 * returns: non-NULL for success; Null otherwise
 */

webpage_t *pageload(int id, char *dirnm) {
	char pathname[MAXVAL];
	webpage_t *pagep = NULL;
	int depth, htmllen, i=0;
	FILE *fp;
	char url[MAXURL];
 	char garbage;
		
	sprintf(pathname,"./%s/%d",dirnm,id);

	if((access(pathname, F_OK) != 0 || access(pathname, R_OK != 0))) {
		printf("Invalid path\n");
		return NULL;
	}

	fp = fopen(pathname, "r");
	if(fp == NULL)
		return NULL;
	
	fscanf(fp, "%s", url);
	fscanf(fp, "%d", &depth);
	fscanf(fp, "%d", &htmllen);

	char html[htmllen];
	printf("%d\n",htmllen);
	garbage = fgetc(fp);
	
	do{
		html[i] = fgetc(fp);
		i=i+1;
	}while(1);
	
	pagep = webpage_new(url, depth, html);

	fclose(fp);
	
	return pagep;
}
