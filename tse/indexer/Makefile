
CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib
LIBS=-lutils -lcurl

all: index.test indexer7

#indexer:
#			gcc $(CFLAGS) indexer.c $(LIBS) -o $@

#indexer4:
#			gcc $(CFLAGS) indexer4.c $(LIBS) -o $@

indexer7:
			gcc $(CFLAGS) indexer7.c $(LIBS) -o $@

#indexsave1.test:
#			gcc $(CFLAGS) indexsave1.test.c $(LIBS) -o $@

index.test:
			gcc $(CFLAGS) index.test.c $(LIBS) -o $@

run:
			./indexer7 pages new_index

clean: 
			rm -f *.o index.test indexer7
