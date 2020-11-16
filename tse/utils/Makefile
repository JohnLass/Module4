CFLAGS=-Wall -pedantic -std=c11 -g
OFILES=lhash.o lqueue.o queue.o hash.o webpage.o pageio.o indexio.o listfun.o lindexio.o

all:	$(OFILES)
			ar cr ../lib/libutils.a $(OFILES)

%.o:  %.c %.h
			gcc $(CFLAGS) -c $<  

clean:
			rm -f *.o



