
CFLAGS=-Wall -pedantic -std=c11 -I../utils -L../lib
LIBS=-lutils -lcurl -lpthread

all: lhopen.test lhput1.test lhput2.test lhsearch.test lhapply1.test lhapply2.test lhremove2.test thread1.test thread2.test

lhopen.test:
			gcc $(CFLAGS) lhopen.test.c $(LIBS) -o $@

lhput1.test:
			gcc $(CFLAGS) lhput1.test.c $(LIBS) -o $@

lhput2.test:
			gcc $(CFLAGS) lhput2.test.c $(LIBS) -o $@

lhsearch.test:
			gcc $(CFLAGS) lhsearch.test.c $(LIBS) -o $@

lhapply1.test:
			gcc $(CFLAGS) lhapply1.test.c $(LIBS) -o $@

lhapply2.test:
			gcc $(CFLAGS) lhapply2.test.c $(LIBS) -o $@

lhremove2.test:
			gcc $(CFLAGS) lhremove2.test.c $(LIBS) -o $@

thread1.test:
			gcc $(CFLAGS) thread1.test.c $(LIBS) -o $@

thread2.test:
			gcc $(CFLAGS) thread2.test.c $(LIBS) -o $@

clean: 
			rm -f *.o lhopen.test lhput1.test lhput2.test lhsearch.test lhapply1.test lhapply2.test lhremove2.test thread1.test thread2.test
