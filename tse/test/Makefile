
CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib
LIBS=-lutils -lcurl

all: pageio.test

pageio.test:
			gcc $(CFLAGS) pageio.test.c $(LIBS) -o $@



clean: 
			rm -f *.o pageio.test
