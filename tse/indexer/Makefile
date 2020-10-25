
CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib
LIBS=-lutils -lcurl

all: indexer indexer4

indexer:
			gcc $(CFLAGS) indexer.c $(LIBS) -o $@

indexer4:
			gcc $(CFLAGS) indexer4.c $(LIBS) -o $@

clean: 
			rm -f *.o indexer indexer4
