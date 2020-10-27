
CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib
LIBS=-lutils -lcurl

all: indexer indexer4 indexsave1.test

indexer:
			gcc $(CFLAGS) indexer.c $(LIBS) -o $@

indexer4:
			gcc $(CFLAGS) indexer4.c $(LIBS) -o $@

indexsave1.test:
			gcc $(CFLAGS) indexsave1.test.c $(LIBS) -o $@

clean: 
			rm -f *.o indexer indexer4 indexsave1.test
