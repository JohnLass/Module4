
CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib
LIBS=-lutils -lcurl

all: crawler crawler2

crawler:
			gcc $(CFLAGS) crawler.c $(LIBS) -o $@

crawler2:
			gcc $(CFLAGS) crawler2.c $(LIBS) -o $@

run:
			./crawler2 https://thayer.github.io/engs50/ pages 1

clean: 
			rm -f *.o crawler crawler2
