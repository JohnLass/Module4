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


int main(void){
	char input[100];
	char query[QLEN][WLEN];
	char *word;
	int flag=0;
	printf("> ");

	while(fgets(input, 1000, stdin) != NULL){
		//checking for return key
		if(strcmp(input,"\n")!=0){
			word = strtok(input, " ");
			int qlen=0;
			while(word!=NULL){
				strcpy(query[qlen],word);
				word = strtok(NULL," ");
				if(checkString(word)==0){
					flag=1;
				}
				qlen++;
			}
			//checking for invalid characters
			if(flag==0){
				for(int i=0; i<qlen; i++){
					char *holder = NormalizeQword(query[i]);
					printf("%s ",holder);
				}
			}else{
				printf("[Invalid Query]");
			}
			printf("> ");
		} else {
			printf("> ");
		}
	}
	
	exit(EXIT_SUCCESS);
}
	
int checkString(char *str){
	if(str==NULL) {
		return -1;
	}
	int length = strlen(str);
	for(int i=0; i<(length-1); i++){
		if(isalpha(str[i])==0){
			return 0;
		}	
	}
	return 1;
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
