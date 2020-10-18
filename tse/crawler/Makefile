
CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib
LIBS=-lutils -lcurl

crawler:
			gcc $(CFLAGS) crawler.c $(LIBS) -o $@


clean: 
			rm -f *.o crawler
