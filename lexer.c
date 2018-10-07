/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#include "error.h"
#include "lexer.h"

FILE* source;

void get_source(FILE *f)
{
	source = f;
}

int get_next_token(Token_t *token) // konecny automat, v podstate while cyklus, ve kterem je switch, nacitame znaky, jakmile urcime token tak ho vratime nebo najdeme blbost a vratime ER_LEX
{
	int current_status = STATE_START;
	if (source == NULL)
	{
		return ER_INTERNAL;
	}
	
	while(true){
		char c = getc(source); // read characters one by one
		
		switch(current_status){
			case(STATE_START): // first comparison
				if(isspace(c)){
					current_status = STATE_START;
				}
				else if(c == '*'){
					token->token = TYPE_MUL;
					current_status = STATE_START;
				}
				else if(c == '+'){
					token->token = TYPE_PLUS;
					current_status = STATE_START;
				}
				else if(c == '/'){
					token->token = TYPE_DIV;
					current_status = STATE_START;
				}
				else if(c == '-'){
					token->token = TYPE_MINUS;
					current_status = STATE_START;
				}
				else if(c == '<'){
					token->token = TYPE_LTN;
					current_status = STATE_LESS_THAN;
				}
				else if(c == '>'){
					token->token = TYPE_MTN;
					current_status = STATE_GREATER_THAN;
				}
				else if(c == '='){
					current_status = STATE_ASSIGN;
				}
				else if(c == '#'){
					token->token = TYPE_COMMENT;
					current_status = STATE_LINE_COMMENTARY;
				}
				else if(c == '('){
					token->token = TYPE_LEFT_BRACKET;
					;
				}
				else if(c == ')'){
					token->token = TYPE_RIGHT_BRACKET;
					;
				}
				else if(c == ','){
					token->token = TYPE_COMMA;
					;
				}
				else if(c == '!'){
					current_status = STATE_EXCLAMATION_MARK;
					;
				}
				else if(isdigit(c)){
					if(c == '0'){
						current_status = STATE_FIRST_ZERO;
					}
					else{
						current_status = STATE_FIRST_NONZERO;
					}
				}
				else{
					; // ERROR
				}
				break;
			case(STATE_LESS_THAN):
				if(c == '='){
					token->token = TYPE_LEQ;
					; // <=
				}
				else{
					token->token = TYPE_LTN;
					current_status = STATE_START; // <
				}
				break;
			case(STATE_GREATER_THAN):
				if(c == '='){
					token->token = TYPE_MEQ;
					; // >=
				}
				else{
					token->token = TYPE_MTN;
					; // <
				}
				break;
			case(STATE_ASSIGN):
				if(c == '='){
					token->token = TYPE_EQ;
					; // ==
				}
				else{
					; // =
				}
				break;
			case(STATE_LINE_COMMENTARY):
				if(c == '\n'){
					token->token = TYPE_COMMENT;
					current_status = STATE_START;
				}
				else{
					current_status = STATE_LINE_COMMENTARY;
				}
				break;
			case(STATE_EXCLAMATION_MARK):
				if(c == '='){
					token->token = TYPE_NEQ;
				}

		}
		if(c == EOF){
			break;
		}
	}
	return 0;
}