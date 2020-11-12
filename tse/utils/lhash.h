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
void lhclose(lhashtable_t *htp);
