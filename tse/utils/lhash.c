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
/* 
 * SuperFastHash() -- produces a number between 0 and the tablesize-1.
 * 
 * The following (rather complicated) code, has been taken from Paul
 * Hsieh's website under the terms of the BSD license. It's a hash
 * function used all over the place nowadays, including Google Sparse
 * Hash.
 */
#define get16bits(d) (*((const uint16_t *) (d)))


typedef struct lht {
	struct hashtable_t *hasht;
  pthread_mutex_t lock;
} lht_t;

static uint32_t SuperFastHash (const char *data,int len,uint32_t tablesize) {
  uint32_t hash = len, tmp;
  int rem;
  
  if (len <= 0 || data == NULL)
		return 0;
  rem = len & 3;
  len >>= 2;
  /* Main loop */
  for (;len > 0; len--) {
    hash  += get16bits (data);
    tmp    = (get16bits (data+2) << 11) ^ hash;
    hash   = (hash << 16) ^ tmp;
    data  += 2*sizeof (uint16_t);
    hash  += hash >> 11;
  }
  /* Handle end cases */
  switch (rem) {
  case 3: hash += get16bits (data);
    hash ^= hash << 16;
    hash ^= data[sizeof (uint16_t)] << 18;
    hash += hash >> 11;
    break;
  case 2: hash += get16bits (data);
    hash ^= hash << 11;
    hash += hash >> 17;
    break;
  case 1: hash += *data;
    hash ^= hash << 10;
    hash += hash >> 1;
  }
  /* Force "avalanching" of final 127 bits */
  hash ^= hash << 3;
  hash += hash >> 5;
  hash ^= hash << 4;
  hash += hash >> 17;
  hash ^= hash << 25;
  hash += hash >> 6;
  return hash % tablesize;
}

/*
 * lqopen -- opens a queue, and makes an associated lock
 */
lhashtable_t* lhopen(uint32_t hsize) {
	ht_t ht = hopen(hsize);
	if(!(lht_t *lht = (lht_t *) malloc(sizeof(lht_t))))
		return NULL;
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
		pthread_mutex_destroy(&ptr->lock);
		free(lhtp);
    return;
}


int32_t lhput(lhashtable_t *htp, void *ep, const char *key, int keylen) {
	if(htp == NULL || ep == NULL || key == NULL)
		return -1;
	lht_t ht = (lht_t *) htp;
	if((pthread_mutex_trylock(&ht->lock)) != 0){
		printf("Trying to access locked hash table");
		return 0;
	}

	hput(ht->hasht,ep,key,keylen);
	pthread_mutex_unlock(&ht->lock);
	return 0;
}
	
void *lhsearch(lhashtable_t *htp, bool (*searchfn)(void* elementp, const void* searchkeyp), const char *key, int32_t keylen){

	if(htp == NULL || searchfn == NULL || key == NULL)
		return NULL;

	lht_t ht = (lht_t *) htp;

	if((pthread_mutex_trylock(&ht->lock)) != 0){
		printf("Trying to access locked hash table");
		return NULL;
	}
	void *search;
	search = hsearch(ht->hasht,searchfn,key,keylen);
	pthread_mutex_unlock(&ht->lock);
	return search;
}


void lhapply(lhashtable_t *htp, void(*fn)(void* ep)){
	if(htp == NULL || fn == NULL)
		return;

	lht_t ht = (lht_t *) htp;
	if((pthread_mutex_trylock(&ht->lock)) != 0){
		printf("Trying to access locked hash table");
		return NULL;
	}

	happly(ht->hasht,fn);
	pthread_mutex_unlock(&ht->lock);
	return;
}

void *hremove(lhashtable_t *htp, bool(*searchfn)(void *elementp, const void* searchkeyp), const char *key, int32_t keylen) {

	if(htp == NULL || searchfn == NULL || key == NULL)
		return;

	lht_t ht = (lht_t *) htp;
	if((pthread_mutex_trylock(&ht->lock)) != 0){
		printf("Trying to access locked hash table");
		return NULL;
	}

	hremove(ht->hasht, searchfn, key, keylen);
	pthread_mutex_unlock(&ht->lock);
	return;
}
