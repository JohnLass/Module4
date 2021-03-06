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

/*Creates and saves an index */
int indexsave(hashtable_t *htp, char *filename);

/*Loads specified index file into indexnm*/
int indexload(hashtable_t *htp, char *fname);
