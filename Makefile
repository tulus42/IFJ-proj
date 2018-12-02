CC=gcc
CFLAGS=-std=c99 -g -pedantic -Wall -Wextra

all: lexer.c dynamic_string.c parser.c tmp_main.c expression.c symtable.c instructions.c
	$(CC) $(CFLAGS) lexer.c dynamic_string.c parser.c tmp_main.c expression.c symtable.c instructions.c -o ifj2018

<<<<<<< HEAD
test:
	./test.sh

=======
>>>>>>> parent of 4c8da57... Updated makefile
clean:
	rm -f *.o tmp_main