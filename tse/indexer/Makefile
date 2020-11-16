
CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib
LIBS=-lutils -lcurl -lpthread

all: index.test indexer7 indexer8 

#indexer:
#			gcc $(CFLAGS) indexer.c $(LIBS) -o $@

#indexer4:
#			gcc $(CFLAGS) indexer4.c $(LIBS) -o $@

indexer7:
			gcc $(CFLAGS) indexer7.c $(LIBS) -o $@

indexer8:
			gcc $(CFLAGS) indexer8.c $(LIBS) -o $@

#indexsave1.test:
#			gcc $(CFLAGS) indexsave1.test.c $(LIBS) -o $@

index.test:
			gcc $(CFLAGS) index.test.c $(LIBS) -o $@

run:
			valgrind ./indexer8 pages new_index 2

clean: 
			rm -f *.o index.test indexer7 indexer8 
