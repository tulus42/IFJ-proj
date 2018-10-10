CFLAGS=-std=c99
tmp_main_lexer: tmp_main_lexer.o
lexer: lexer.o
dynamic_string: dynamic_string.o
clean:
	-rm lexer lexer.o tmp_main_lexer tmp_main_lexer.o dynamic_string synamic_string.o
