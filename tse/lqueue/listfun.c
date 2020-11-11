/* listfun.c --- 
 * 
 * 
 * Author: John J. Lass
 * Created: Fri Oct  2 14:29:12 2020 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include "listfun.h" 

car_t *makecar(char *platep,double price,int year) {
	 car_t *cp=NULL;
	 if((cp = (car_t*)malloc(sizeof(car_t)))==NULL) {
		 printf("[Error: malloc failed allocating car]\n");
		 return NULL;
	 }
	 cp->next = NULL;
	 strcpy(cp->plate,platep);
	 cp->price=price;
	 cp->year=year;
	 printf("Car created\n");
	 return cp;
 }

void print_node(void *vp){
	if(vp == NULL){
		printf("Passed a null pointer\n");
	}else{
		car_t *cp = (car_t *) vp;
		printf("The car's licence plate is: %s\n", cp->plate);
		printf("The car's price is: %lf\n", cp->price);
		printf("The car's year is: %d\n", cp->year);
	}

}

void make_list(void){
	car_t * firstp = makecar("8008135", 60.4, 2020);
	car_t * secondp = makecar("p", 6020.2, 2003);
	car_t * lastp = makecar("rfc47", 3932.9, 20120);
	lput(lastp);
	lput(secondp);
	lput(firstp);
	return;
}

bool checkcar(car_t *cp, char *platep, double price, int year){
	if(cp->price != price)
		return false;
	if(strcmp(cp->plate,platep) != 0)
		return false;
	if(cp->year != year)
		return false;
	return true;
}

	
bool search_plate (void* elementp, const void* keyp) {
	int ret;
	if(elementp!=NULL){
		car_t *car1 = (car_t *) elementp;
		if(keyp != NULL){
			car_t *key = (car_t *) keyp;
			ret = strcmp(car1->plate,key->plate);
			if(ret!=0)
				return false;
			return true;
		}
	}
	return false;	
}

bool search_year (void* elementp, const void* keyp) {
	bool ret = false;
	if(elementp!=NULL){
		car_t *car1 = (car_t *) elementp;
		if(keyp != NULL){
			car_t *key = (car_t *) keyp;
			if(car1->year == key->year)
				ret = true;	
		}
	}
	return ret;	
}

bool search_price (void* elementp, const void* keyp) {
	bool ret = false;
	if(elementp!=NULL){
		car_t *car1 = (car_t *) elementp;
		if(keyp != NULL){
			car_t *key = (car_t *) keyp;
			if(car1->price == key->price)
				ret = true;	
		}
	}
	return ret;	
}

