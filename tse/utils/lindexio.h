#pragma once
/* indexio.h --- 
 * 
 * 
 * Author: John J. Lass
 * Created: Tue Oct 27 13:48:00 2020 (-0400)
 * Version: 
 * 
 * Description: Header file for indexing webpages and creating Hastables from
 *the HTML 
 * 
 */

#include <queue.h>
#include <hash.h>
#include <lhash.h>
#include <lqueue.h>

/*Creates and saves an index */
int lindexsave(lhashtable_t *lhtp, char *filename);

/*Loads specified index file into indexnm*/
int lindexload(lhashtable_t *lhtp, char *fname);
