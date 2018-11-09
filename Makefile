CFLAGS=-std=c99 -g
CC = gcc
make:
	${CC} ${CFLAGS} tmp_main.c -o tmp_main

clean:
	-rm tmp_main tmp_main.o
