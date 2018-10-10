CFLAGS=-std=c99
tmp_main_lexer: tmp_main_lexer.o
lexer: lexer.o
clean:
	-rm lexer lexer.o tmp_main_lexer tmp_main_lexer.o