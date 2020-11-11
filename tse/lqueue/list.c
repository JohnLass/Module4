/* list.c --- 
 * 
 * 
 * Author: John J. Lass, Robert Carangelo, Tyler Neath
 * Created: Thu Oct  1 18:06:18 2020 (-0400)
 * Version: 
 * 
 * Description: Functions to manipulate list of cars
 * 
 */
#include "list.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "listfun.h"

static car_t *front=NULL; 
// 2 cases; 1 cp is null , we don't care wht happens and we leave
// the other case: cp! = NULL and we want to add it  to the list
// adding to list: if front is null or already something there
int32_t lput(car_t *cp) {

	if(cp==NULL){
		
		return -1;

	}else{

		cp->next = front;
		front=cp;
		return 0;

	}

}
	
	
car_t *lget()  {

	car_t *fp;
	
	if(front==NULL) {

		printf("List is empty \n");
		return NULL;

	}else if(front!=NULL) {

		fp=front;
		free(front);
		front = fp->next;
		return fp;
		
			}else{

		printf("Error using function 'get()'\n");
		return NULL;

	}
	
}

void lapply(void (*fn)(car_t *cp)) {

	car_t *incp=NULL;

	for(incp=front; incp!=NULL; incp= incp->next){

		fn(incp);

	}

}

car_t *lremove(char *platep) {

	car_t *incp=NULL;
	car_t *fp=NULL;
	car_t *prevp=front;
	int ret=0;
	int count=0;
	int loop_count = 0;
	
	if(platep!=NULL){

		//loop through linked list
		for(incp=front; incp!=NULL; incp=incp->next){

			//check if the plate matches
			ret=strcmp(incp->plate,platep);
			
			if((ret==0)) {
				
				//set return pointer to this one in the list
				fp = incp;
				//note that we have found it
				count = count + 1;
				//if its the first thing set the front to the second one, if not set previus->next to this one -> next
				if(loop_count == 0){
					front = front->next;
				}else{
					prevp->next = incp->next;
				}
				
			}

			//if we havent found it yet increment the previous one
			if(count==0){
				prevp = incp;
			}
			//increment loop count
			loop_count = loop_count + 1;
		}

	}else{
		
		printf("Invalid arg");
		return NULL;

	}

	if((count==0)){
		printf("Plate not found in list");
		return NULL;
	}

	return fp;
	
}
