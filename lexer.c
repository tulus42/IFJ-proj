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
#include "dynamic_string.c"

FILE* source;

void get_source(FILE *f)
{
	source = f;
}

void change_state(int * current_state, int next_state){
	*current_state = next_state;
}

void keywords(string* string_ptr, Token_t* token){
	if(compare_strings(string_ptr, "def")){
		token->token = TYPE_KEYWORD;
		token->attr.keyword = KEYWORD_DEF;
	}
	else if(compare_strings(string_ptr, "do")){
		token->token = TYPE_KEYWORD;
		token->attr.keyword = KEYWORD_DO;
	}
	else if(compare_strings(string_ptr, "else")){
		token->token = TYPE_KEYWORD;
		token->attr.keyword = KEYWORD_ELSE;
	}
	else if(compare_strings(string_ptr, "end")){
		token->token = TYPE_KEYWORD;
		token->attr.keyword = KEYWORD_END;
	}
	else if(compare_strings(string_ptr, "if")){
		token->token = TYPE_KEYWORD;
		token->attr.keyword = KEYWORD_IF;
	}
	else if(compare_strings(string_ptr, "not")){
		token->token = TYPE_KEYWORD;
		token->attr.keyword = KEYWORD_NOT;
	}
	else if(compare_strings(string_ptr, "nil")){
		token->token = TYPE_KEYWORD;
		token->attr.keyword = KEYWORD_NIL;
	}
	else if(compare_strings(string_ptr, "then")){
		token->token = TYPE_KEYWORD;
		token->attr.keyword = KEYWORD_THEN;
	}
	else if(compare_strings(string_ptr, "while")){
		token->token = TYPE_KEYWORD;
		token->attr.keyword = KEYWORD_WHILE;
	}
	else{
		token->token = TYPE_IDENTIFIER;
		/// MA TO IST DO ATTR STRING????? ALEBO NE??????
	}

}


int get_next_token(Token_t *token) // konecny automat, v podstate while cyklus, ve kterem je switch, nacitame znaky, jakmile urcime token tak ho vratime nebo najdeme blbost a vratime ER_LEX
{
	// current state is start
	int current_status = STATE_START;
	// variable for string
	string* string_ptr = NULL;
	init_struc_pointer(string_ptr);

	if (source == NULL)
	{
		return ER_INTERNAL;
	}

	while(true){
		char c = getc(source); // read characters one by one
		printf("%c\n", c);
		;
		switch(current_status){
			
			// all possible first states
			case(STATE_START):
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
						// can continue, first char is
						//allocate_string(string_ptr);
						//add_char(string_ptr, c);
						change_state(&current_status, STATE_START);
					}
					else{
						// not allowed to start with uppercase!!!
					}
				}
				else if(c == EOF){
					token->token = TYPE_EOF;
					change_state(&current_status, STATE_EOF);
				}
				else{
					printf("ERROR\n"); // ERROR
				}
				break;

			// previous char was <
			case(STATE_LESS_THAN):
				if(c == '='){ // <=
					token->token = TYPE_LEQ;
					change_state(&current_status, STATE_START);
				
				}
				else{ // only <, puts the previous char back to buffer
					token->token = TYPE_LTN;
					ungetc(c, source);
					change_state(&current_status, STATE_START);
				}
				break;

			// previous char was >
			case(STATE_GREATER_THAN):
				if(c == '='){
					token->token = TYPE_MEQ;
					change_state(&current_status, STATE_START);

				}
				else{
					token->token = TYPE_MTN;
					ungetc(c, source);
					change_state(&current_status, STATE_START);
				}
				break;

			// previous char was =
			case(STATE_ASSIGN):
				if(c == '='){
					token->token = TYPE_EQ;
					change_state(&current_status, STATE_START); // ==
				}
				else{
					ungetc(c, source);
					current_status = STATE_START; // =
				}
				break;

			// line commentary
			case(STATE_LINE_COMMENTARY):
				if(c == '\n'){
					token->token = TYPE_COMMENT;
					change_state(&current_status, STATE_START);
				}
				else{
					current_status = STATE_LINE_COMMENTARY;
				}
				break;

			// previous char was !, only != can be accepted
			case(STATE_EXCLAMATION_MARK):
				if(c == '='){
					token->token = TYPE_NEQ;
					change_state(&current_status, STATE_START);
				}
				else{
					; // error - invalid character
				}
				break;
			
			// array of chars
			case(STATE_NEXT_CHARS):
				if(isalpha(c) || isdigit(c) || c == '_'){
					add_char(string_ptr, c); // ADDING NEW CHARS TO THE STRING
				}
				else if(c == '?' || c == '!'){
					add_char(string_ptr, c);
					change_state(&current_status, STATE_LAST_CHAR);
					// this has to be the end of string
				}
				else{
					// TUTO JE KONIEC
					ungetc(c, source);
					keywords(string_ptr, token);
					/// HAVE TO COMPARE IT WITH ALL THE KEYWORDS!!!! TODO
					free_string(string_ptr);
				}
				break;

			case(STATE_LAST_CHAR):
				if(isalpha(c) || isdigit(c) || c == '_'){
					ungetc(c, source);
					free_string(string_ptr);
					change_state(&current_status, STATE_START);
					// ERROR
					; // NOT ALL WELL, CANT BE IDENTIFIER
				}
				else{
					ungetc(c, source);
					change_state(&current_status, STATE_START);
					token->token = TYPE_IDENTIFIER;
					;// ALL IS WELL, CAN BE IDENTIFIER
				}
				break;

		}
		//printf("Toto je token %d\n", token->token);
		//printf("Som vo while a current status je %d\n", current_status);
		if(c == EOF){
			break;
		}
		
	}
	free_struc_pointer(string_ptr);
	return 0;
}