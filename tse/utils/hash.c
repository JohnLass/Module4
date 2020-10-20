/* 
 * hash.c -- implements a generic hash table as an indexed set of queues.
 *
 */
#include <stdint.h>
#include "hash.h"
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* 
 * SuperFastHash() -- produces a number between 0 and the tablesize-1.
 * 
 * The following (rather complicated) code, has been taken from Paul
 * Hsieh's website under the terms of the BSD license. It's a hash
 * function used all over the place nowadays, including Google Sparse
 * Hash.
 */
#define get16bits(d) (*((const uint16_t *) (d)))


typedef struct ht {
	queue_t **qTable;
  uint32_t hsize;
} ht_t;
 
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

hashtable_t *hopen(uint32_t hsize) {
  //memory allocation 
  ht_t *ht;
  if (!(ht=(ht_t*)malloc(sizeof(ht_t)))) {
      printf("Error: Failure allocating memory\n");
      return NULL;
  }
  if(!(ht->qTable=malloc(sizeof(void *)*hsize))){
    printf("Error: Failure allocating memory\n");
    return NULL;
  }

  //setting hashsize
  ht->hsize=hsize;
  
  //setting queue pointers
  queue_t **hold = ht->qTable;
  for(int i=0; i<hsize; i++){
    hold[i] = qopen();
  }

  return (hashtable_t*)ht;
}

void hclose(hashtable_t *htp){
  if(htp!=NULL){
		ht_t *ptr = (ht_t*)htp;
		queue_t **hold = ptr->qTable;
    for(int i=0; i<ptr->hsize; i++){
      qclose(hold[i]);
    }
  
  free(ptr->qTable);
  free(ptr);
	}
}
int32_t hput(hashtable_t *htp, void *ep, const char *key, int keylen){
  ht_t *ptr = htp;
  int value = 1;
  if((ptr!=NULL) && (ep!=NULL)){
    uint32_t pos;
    pos = SuperFastHash(key,keylen,ptr->hsize);
    queue_t **hold = ptr->qTable;
    value = qput(hold[pos],ep);
  }

  return value;
  
}

	

void *hsearch(hashtable_t *htp, bool (*searchfn)(void* elementp, const void* searchkeyp), const char *key, int32_t keylen){
	if(htp!=NULL && key!=NULL && keylen>=0){
		ht_t *ptr = (ht_t*)htp;
		uint32_t pos;
		queue_t **hold = ptr->qTable;
		void *data=(void*)key;
		pos = SuperFastHash(key,keylen,ptr->hsize);
		void *rtrn=qsearch(hold[pos],searchfn,data);
		return rtrn;
	}
	printf("ERROR With Hash/key/keylen");
	return NULL;
}

void happly(hashtable_t *htp, void (*fn)(void* ep)){
	if(htp != NULL && fn != NULL){
		ht_t *ptr = (ht_t *) htp;
		for(int i = 0; i < ptr->hsize; i++){
			qapply(ptr->qTable[i],fn);		
		}
	}
	else{
		printf("NULL arg\n");
	}
}

void *hremove(hashtable_t *htp, bool(*searchfn)(void* elementp, const void* searchkeyp), const char *key, int32_t keylen){

	if(htp!=NULL && key!=NULL && keylen>=0){
		ht_t *ptr = (ht_t*)htp;
		uint32_t pos;
		queue_t **hold = ptr->qTable;
		void *data=(void*)key;
		pos = SuperFastHash(key,keylen,ptr->hsize);
		void *rtrn=qremove(hold[pos],searchfn,data);
		return rtrn;
	}
	printf("ERROR With Hash/key/keylen");
	return NULL;
}
