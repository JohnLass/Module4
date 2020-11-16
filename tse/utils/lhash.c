/* lhash.c --- 
 * 
 * 
 * Author: Tyler I. Neath, Robert F. Carangelo III, John J. Lass
 * Created: Thu Nov 12 14:45:47 2020 (-0500)
 * Version: 1.0
 * 
 * Description: functions to create and manipulate a locked hashtable. 
 * 
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "listfun.h"
#include "queue.h"
#include "lhash.h"
#include "hash.h"



typedef struct lht {
	struct hashtable_t *hasht;
  	pthread_mutex_t lock;
} lht_t;

/*
 * lqopen -- opens a queue, and makes an associated lock
 */
lhashtable_t* lhopen(uint32_t hsize) {
	hashtable_t* ht = hopen(hsize);
	lht_t* lht;
	if (!(lht=(lht_t*)malloc(sizeof(lht_t)))){
		return NULL;
	}
	lht->hasht = ht;
	pthread_mutex_init(&lht->lock, NULL);
	return (lhashtable_t*)lht;
}  

/*
 * lqclose -- opens a queue, and makes an associated lock
 */
void lhclose(lhashtable_t *lhtp) {
    if(lhtp == NULL){
        return;
    }
		lht_t *lht = (lht_t *) lhtp;
		hclose(lht->hasht);
		pthread_mutex_destroy(&lht->lock);
		free(lhtp);
    return;
}


int32_t lhput(lhashtable_t *lhtp, void *ep, const char *key, int keylen) {
	if(lhtp == NULL || ep == NULL || key == NULL)
		return -1;
	lht_t* ht = (lht_t *) lhtp;
	if((pthread_mutex_trylock(&ht->lock)) != 0){
		printf("Trying to access locked hash table");
		return 0;
	}

	hput(ht->hasht,ep,key,keylen);
	pthread_mutex_unlock(&ht->lock);
	return 0;
}
	
void *lhsearch(lhashtable_t *lhtp, bool (*searchfn)(void* elementp, const void* searchkeyp), const char *key, int32_t keylen){

	if(lhtp == NULL || searchfn == NULL || key == NULL)
		return NULL;

	lht_t* ht = (lht_t *) lhtp;

	if((pthread_mutex_trylock(&ht->lock)) != 0){
		printf("Trying to access locked hash table");
		return NULL;
	}
	void *search = hsearch(ht->hasht,searchfn,key,keylen);
	pthread_mutex_unlock(&ht->lock);
	return search;
}


void lhapply(lhashtable_t *lhtp, void(*fn)(void* ep)){
	if(lhtp == NULL || fn == NULL)
		return;

	lht_t* ht = (lht_t *) lhtp;
	if((pthread_mutex_trylock(&ht->lock)) != 0){
		printf("Trying to access locked hash table");
		return;
	}

	happly(ht->hasht,fn);
	pthread_mutex_unlock(&ht->lock);
}

void *lhremove(lhashtable_t *lhtp, bool(*searchfn)(void *elementp, const void* searchkeyp), const char *key, int32_t keylen) {

	if(lhtp == NULL || searchfn == NULL || key == NULL)
		return NULL;

	lht_t* ht = (lht_t *) lhtp;
	if((pthread_mutex_trylock(&ht->lock)) != 0){
		printf("Trying to access locked hash table");
		return NULL;
	}
	void *rtrn = hremove(ht->hasht, searchfn, key, keylen);
	pthread_mutex_unlock(&ht->lock);
	return rtrn;
}
