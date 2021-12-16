CC=gcc
CFLAGS=-std=c99 -g -pedantic -Wall -Wextra

all: scanner.c dynamic_string.c parser.c main.c expression.c symtable.c instructions.c
	$(CC) $(CFLAGS) scanner.c dynamic_string.c parser.c main.c expression.c symtable.c instructions.c -o ifj2018

clean:
	rm -f *.o ifj2018
