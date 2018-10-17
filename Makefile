CFLAGS=-std=c99
tmp_main_lexer: tmp_main_lexer.o
lexer: lexer.o
dynamic_string: dynamic_string.o
clean:
	-rm tmp_main_lexer tmp_main_lexer.o
