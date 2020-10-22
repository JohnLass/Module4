
CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib
LIBS=-lutils -lcurl

all: indexer

indexer:
			gcc $(CFLAGS) indexer.c $(LIBS) -o $@


clean: 
			rm -f *.o indexer
