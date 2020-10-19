
CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib
LIBS=-lutils -lcurl

all: crawler crawler2

crawler:
			gcc $(CFLAGS) crawler.c $(LIBS) -o $@

crawler2:
			gcc $(CFLAGS) crawler2.c $(LIBS) -o $@


clean: 
			rm -f *.o crawler crawler2
