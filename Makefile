CFLAGS=-std=c99
tmp_main: tmp_main.o
lexer: lexer.o
dynamic_string: dynamic_string.o
parser: parser.o
clean:
	-rm tmp_main tmp_main.o
