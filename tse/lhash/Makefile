
CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib
LIBS=-lutils -lcurl -lpthread

all: lhopen.test

lhopen.test:
			gcc $(CFLAGS) lhopen.test.c $(LIBS) -o $@

clean: 
			rm -f *.o lhopen.test 
