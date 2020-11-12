
CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib
LIBS=-lutils -lcurl -lpthread

all: lqopen.test lqput1.test lqget1.test thread1.test lqapply1.test thread2.test

lqopen.test:
			gcc $(CFLAGS) lqopen.test.c $(LIBS) -o $@

lqput1.test:
			gcc $(CFLAGS) lqput1.test.c $(LIBS) -o $@

lqget1.test:
			gcc $(CFLAGS) lqget1.test.c $(LIBS) -o $@

thread1.test:
			gcc $(CFLAGS) thread1.test.c $(LIBS) -o $@

thread2.test:
			gcc $(CFLAGS) thread2.test.c $(LIBS) -o $@

lqapply1.test:
			gcc $(CFLAGS) lqapply1.test.c $(LIBS) -o $@

clean: 
			rm -f *.o lqopen.test lqput1.test lqget1.test thread1.test lqapply.test thread2.test
