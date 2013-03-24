
CC = gcc
CFLAGS = -lm -g -Wall -O0 -Wshadow -Wwrite-strings -ansi -pedantic-errors 
PROGS = test 

.PHONY: all clean

all: $(PROGS)

clean:
	rm -f *.o $(PROGS)

test: test.c matrices.o
matrices.o: matrices.h 
