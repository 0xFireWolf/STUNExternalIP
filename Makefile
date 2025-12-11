CC=gcc
CLIBS=-lc
CFLAGS=-g -Werror-implicit-function-declaration -pedantic -std=c11

OBJS=STUNExternalIP.o main.o

STUNExternalIP: $(OBJS)
	$(CC) -g -o STUNExternalIP $(OBJS)

main.o: main.c
STUNExternalIP.o: STUNExternalIP.c STUNExternalIP.h

clean:
	rm -f *.o
	rm -f STUNExternalIP