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

void keywords(struct string_t *string_ptr, Token_t* token){
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
		token->attr.string = string_ptr->s;
		/// MA TO IST DO ATTR STRING????? ALEBO NE??????
	}
	printf("Token pre %s je: %d\n", string_ptr->s, token->token);
}

/*

TODO: 
kde je malloc, treba tam return type bool;
return v každom case?
co mam robit ked identifier zacina s velkym pismenom?
=begin if not match
skontrolovať rozsah int a double?

*/

int free_my_dynamic_string(int lexer_state, struct string_t* string_ptr){
	free_string(string_ptr);
	return 0;
}

int lexer_error(struct string_t* string_ptr){
	free_string(string_ptr);
	printf("Lexer ERROR\n");
	return LEXER_ERROR;
}

int get_next_token(Token_t *token) // konecny automat, v podstate while cyklus, ve kterem je switch, nacitame znaky, jakmile urcime token tak ho vratime nebo najdeme blbost a vratime ER_LEX
{
	// current state is start
	int current_status = STATE_START;
	int registered_input;
	// variable for string struct
	struct string_t string;
	struct string_t *string_ptr = &string;
	allocate_string(string_ptr);

	if (source == NULL)
	{
		return ER_INTERNAL;
	}

	while(true){
		char c = getc(source); // read characters one by one
		//printf("%c\n", c);
		switch(current_status){

			// all possible first states
			case(STATE_START):
				if(isspace(c) || c == '\n'){
					break;
				}
				else if(c == '*'){
					token->token = TYPE_MUL;
				}
				else if(c == '+'){
					token->token = TYPE_PLUS;
				}
				else if(c == '/'){
					token->token = TYPE_DIV;
				}
				else if(c == '-'){
					token->token = TYPE_MINUS;
				}
				else if(c == '('){
					token->token = TYPE_LEFT_BRACKET;
				}
				else if(c == ')'){
					token->token = TYPE_RIGHT_BRACKET;
				}
				else if(c == ','){
					token->token = TYPE_COMMA;
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
				else if(c == '!'){
					change_state(&current_status, STATE_EXCLAMATION_MARK);
				}
				else if(c == '"'){
					change_state(&current_status, STATE_STRING_LITERAL);
				}
				else if(isdigit(c)){
					if(c == '0'){
						change_state(&current_status, STATE_FIRST_ZERO);
					}
					else{
						change_state(&current_status, STATE_FIRST_NONZERO);
					}
				}
				else if(isalpha(c) || c == '_'){
					if(islower(c)){
						// can continue, first char is
						add_char(string_ptr, c);
						change_state(&current_status, STATE_NEXT_CHARS);
					}
					else{
						return lexer_error(string_ptr);
					}
				}
				else if(c == EOF){
					token->token = TYPE_EOF;
					change_state(&current_status, STATE_EOF);
				}
				else{
					return lexer_error(string_ptr);
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
				else if(isalpha(c) && c == 'b'){
					add_char(string_ptr, '=');
					add_char(string_ptr, c);
					change_state(&current_status, STATE_COMMENT_START);
				}
				else{
					ungetc(c, source);
					change_state(&current_status, STATE_START); // =
				}
				break;

			// line commentary
			case(STATE_LINE_COMMENTARY):
				token->token = TYPE_COMMENT;
				if(c == '\n'){
					change_state(&current_status, STATE_START);
				}
				break;

			// check whether input is '=begin'
			case(STATE_COMMENT_START):	
				add_char(string_ptr, c);
				registered_input = strlen(string_ptr->s);

				if(check_comment_begin(registered_input, string_ptr)){
					if(registered_input == strlen("=begin")){
						free_string(string_ptr);
						allocate_string(string_ptr);
						change_state(&current_status, STATE_INSIDE_BLOCK_COMMENT);
					}
				}
				else{
					// Not matching - has to remove the first '=' and save the string ?? TODO
				}
				break;

			// waits for =, everything inside the block comment is ignored
			case(STATE_INSIDE_BLOCK_COMMENT):
				if(c == '='){
					add_char(string_ptr, c);
					change_state(&current_status, STATE_COMMENT_END);
				}
				else{
					token->token = TYPE_COMMENT;
					 // Inside of a block comment gets ignored
				}
				break;

			// check whether input is '=end'
			case(STATE_COMMENT_END):
				add_char(string_ptr, c);
				registered_input = strlen(string_ptr->s);
				if(check_comment_end(registered_input, string_ptr)){
					if(registered_input == strlen("=end")){
						change_state(&current_status, STATE_START);
						free_string(string_ptr);
						allocate_string(string_ptr);
					}
				}
				else{
					clear_string_content(string_ptr); // Not matching - has to remove it completely and waint for another =
					change_state(&current_status, STATE_INSIDE_BLOCK_COMMENT);
				}
				break;

			// previous char was !, only != can be accepted
			case(STATE_EXCLAMATION_MARK):
				if(c == '='){
					token->token = TYPE_NEQ;
					change_state(&current_status, STATE_START);
				}
				else{
					return lexer_error(string_ptr);
				}
				break;
			
			// array of chars
			case(STATE_NEXT_CHARS):
				if(isalpha(c) || isdigit(c) || c == '_'){
					add_char(string_ptr, c); // ADDING NEW CHARS TO THE STRING
				}
				else if(c == '?' || c == '!'){
					;//add_char(string_ptr, c);
					add_char(string_ptr, c);
					change_state(&current_status, STATE_LAST_CHAR);
					// this has to be the end of string
				}
				else{
					// TUTO JE KONIEC
					ungetc(c, source);
					keywords(string_ptr, token);
					/// HAVE TO COMPARE IT WITH ALL THE KEYWORDS!!!! TODO
					//printf("%s\n", string_ptr->s);
					free_string(string_ptr);
					allocate_string(string_ptr);
					change_state(&current_status, STATE_START);
				}
				break;

			// last char can be ! or ?, but it cannot be followed by anything else
			case(STATE_LAST_CHAR):
				if(isalpha(c) || isdigit(c) || c == '_'){
					lexer_error(string_ptr);
				}
				else{ // ALL IS WELL, CAN BE IDENTIFIER
					ungetc(c, source);
					change_state(&current_status, STATE_START);
					keywords(string_ptr, token);
					free_string(string_ptr);
					allocate_string(string_ptr);
				}
				break;
			case(STATE_STRING_LITERAL):
				if(c == '"'){
					token->token = TYPE_STRING_LITERAL;
					token->attr.string = string_ptr->s;
					free_string(string_ptr);
					change_state(&current_status, STATE_START);
					allocate_string(string_ptr);
				}
				else{
					add_char(string_ptr, c);
				}
				break;

			// this should work like a string
			case(STATE_FIRST_ZERO):
				token->token = TYPE_INT;
				token->attr.integer = 0;
				change_state(&current_status, STATE_START);
				break;

			
			case(STATE_FIRST_NONZERO):
				break;

		}
		//printf("Toto je token %d\n", token->token);
		if(c == EOF){
			
			break;
		}
	}
	// toto bude musieť byť inde
	//free_string(string_ptr);

	return LEXER_OK;
}