CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib
LIBS=-lutils -lcurl 

all: testQ testH

testQ:
			gcc $(CFLAGS) testQ.o $(LIBS) -o $@ -lpthread

testH:
			gcc $(CFLAGS) testH.o $(LIBS) -o $@ -lpthread

clean: 
			rm -f testQ testH
