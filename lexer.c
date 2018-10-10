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

void change_state(int * current_state, int next_state){
	*current_state = next_state;
}

//void reserved_or_keywords(){;};
// process_chars

//void process_digits

int get_next_token(Token_t *token) // konecny automat, v podstate while cyklus, ve kterem je switch, nacitame znaky, jakmile urcime token tak ho vratime nebo najdeme blbost a vratime ER_LEX
{
	int current_status = STATE_START;
	if (source == NULL)
	{
		return ER_INTERNAL;
	}

	while(true){
		char c = getc(source); // read characters one by one
		printf("%c\n", c);
		;
		switch(current_status){
			
			case(STATE_START): // first comparison
				if(isspace(c)){
					change_state(&current_status, STATE_START);
				}
				else if(c == '*'){
					token->token = TYPE_MUL;
					change_state(&current_status, STATE_START);
				}
				else if(c == '+'){
					token->token = TYPE_PLUS;
					change_state(&current_status, STATE_START);
				}
				else if(c == '/'){
					token->token = TYPE_DIV;
					change_state(&current_status, STATE_START);
				}
				else if(c == '-'){
					token->token = TYPE_MINUS;
					change_state(&current_status, STATE_START);
				}
				else if(c == '<'){
					token->token = TYPE_LTN;
					change_state(&current_status, STATE_LESS_THAN);
				}
				else if(c == '>'){
					token->token = TYPE_MTN;
					change_state(&current_status, STATE_GREATER_THAN);
				}
				else if(c == '='){
					change_state(&current_status, STATE_ASSIGN);
				}
				else if(c == '#'){
					token->token = TYPE_COMMENT;
					change_state(&current_status, STATE_LINE_COMMENTARY);
				}
				else if(c == '('){
					token->token = TYPE_LEFT_BRACKET;
					change_state(&current_status, STATE_START);
				}
				else if(c == ')'){
					token->token = TYPE_RIGHT_BRACKET;
					change_state(&current_status, STATE_START);
				}
				else if(c == ','){
					token->token = TYPE_COMMA;
					change_state(&current_status, STATE_START);
				}
				else if(c == '!'){
					change_state(&current_status, STATE_EXCLAMATION_MARK);
					;
				}
				else if(isdigit(c)){
					if(c == '0'){
						//current_status = STATE_FIRST_ZERO;
						change_state(&current_status, STATE_START);
					}
					else{
						//current_status = STATE_FIRST_NONZERO;
						change_state(&current_status, STATE_START);
					}
				}
				else if(isalpha(c) || c == '_'){
					if(islower(c)){
						// can continue, first char is OK
						change_state(&current_status, STATE_START);
					}
					else{
						// not allowed to start with uppercase!!!
					}
				}
				else if(c == EOF){
					change_state(&current_status, STATE_EOF);
				}
				else{
					printf("ERROR\n"); // ERROR
				}
				break;
			case(STATE_LESS_THAN):
				if(c == '='){
					token->token = TYPE_LEQ;
					change_state(&current_status, STATE_START);
				
				}
				else{
					token->token = TYPE_LTN;
					change_state(&current_status, STATE_START);
				}
				break;
			case(STATE_GREATER_THAN):
				if(c == '='){
					token->token = TYPE_MEQ;
					change_state(&current_status, STATE_START);

				}
				else{
					token->token = TYPE_MTN;
					change_state(&current_status, STATE_START);
				}
				break;
			case(STATE_ASSIGN):
				if(c == '='){
					token->token = TYPE_EQ;
					change_state(&current_status, STATE_START); // ==
				}
				else{
					current_status = STATE_START; // =
				}
				break;
			case(STATE_LINE_COMMENTARY):
				if(c == '\n'){
					token->token = TYPE_COMMENT;
					change_state(&current_status, STATE_START);
				}
				else{
					current_status = STATE_LINE_COMMENTARY;
				}
				break;
			case(STATE_EXCLAMATION_MARK):
				if(c == '='){
					token->token = TYPE_NEQ;
					change_state(&current_status, STATE_START);
				}

		}
		//printf("Som vo while a current status je %d\n", current_status);
		if(c == EOF){
			break;
		}
		
	}
	return 0;
}