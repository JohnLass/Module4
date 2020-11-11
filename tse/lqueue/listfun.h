#pragma once
/* listfun.h --- 
 * 
 * 
 * Author: John J. Lass
 * Created: Fri Oct  2 12:56:59 2020 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */

#include "list.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>



/*Initializes the list by making front->next point to NULL*/
void listinit();




/*Used to allocate memory and add values to a new list entry*/
car_t *makecar(char*platep,double price,int year);



/*Shifts all list entries and adds a new entry to the beginning of the list*/
car_t *shadd();


/*Prints the car's plate, price and year. Checks to make sure pointer isn't null   */
void print_node(void *vp);

/* Makes a list with three car entries in it*/
void make_list(void);

bool checkcar(car_t *cp, char *platep, double price, int year);

bool search_plate (void* elementp, const void* keyp);

bool search_year (void* elementp, const void* keyp);

bool search_price (void* elementp, const void* keyp);
