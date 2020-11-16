#pragma once
/*
 * lhash.h -- public interface to the lhash module
 */
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

/* representation of a hashtable hidden */
typedef void lhashtable_t;

/* lhopen - create an empty locked hash table */
lhashtable_t *lhopen(uint32_t hsize);

/* lhclose - close and free a locked hash table */
void lhclose(lhashtable_t *lhtp);

/* lhput -- puts an entry into a hash table under designated key 
 * returns 0 for success; non-zero otherwise
 */
int32_t lhput(lhashtable_t *lhtp, void *ep, const char *key, int keylen);

/* lapply -- applies a function to every entry in hash table */
void lhapply(lhashtable_t *lhtp, void (*fn)(void* ep));

/* lhsearch -- searchs for an entry under a designated key using a
 * designated search fn -- returns a pointer to the entry or NULL if
 * not found
 */
void *lhsearch(lhashtable_t *lhtp, 
	      bool (*searchfn)(void* elementp, const void* searchkeyp), 
	      const char *key, 
	      int32_t keylen);

/* lhremove -- removes and returns an entry under a designated key
 * using a designated search fn -- returns a pointer to the entry or
 * NULL if not found
 */
void *lhremove(lhashtable_t *lhtp, 
	      bool (*searchfn)(void* elementp, const void* searchkeyp), 
	      const char *key, 
	      int32_t keylen);