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
#include "testing.h"

FILE* source;
#define LEXER_OK 0

/*
TODO: 
hex_number error
return v každom case?
=begin if not match
skontrolovať rozsah int a double?
check how ungetc work when pushing more chars to the buffer
REMOVE ALL THE DEBUG PRINTS!!!!
*/

/**
 * Sets the source file
 */
void get_source(FILE *f)
{
	source = f;
}

/**
 * Changes current state to the next state by hard copy
 */
void change_state(int * current_state, int next_state){
	*current_state = next_state;
}

/**
 * Compares the entered string and checkes, wheter it's keyword or not
 * If not keyword, it is automatically an identifier
 */
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
	
	if(token->token == TYPE_KEYWORD){
		printf("%s : %s : %s\n", tokens[token->token], string_ptr->s, keyword[token->attr.keyword]);
	}
	else{
		printf("%s : %s\n", tokens[token->token], string_ptr->s);
	}
}

/**
 * Writes error to stderr, returns error type
 */
int lexer_error(struct string_t* string_ptr, int error_type){
	free_string(string_ptr);
	fprintf(stderr, "Lexer ERROR\n");
	return error_type;
}

/**
 * 
 * Finite state machine, it either return a correct token or EL_LEX if there is lexical error
 * 
 */
int get_next_token(Token_t *token) // konecny automat, v podstate while cyklus, ve kterem je switch, nacitame znaky, jakmile urcime token tak ho vratime nebo najdeme blbost a vratime ER_LEX
{
	/* Used variables */
	int current_status = STATE_START; // current state is start
	int registered_input; // counts how many chars have been entered for '=begin' and '=end'

	// variable for string struct
	struct string_t string;
	struct string_t *string_ptr = &string;


	char hex_num[2] = {'\0', '\0'}; // help variable for hexadecimal '\xhh' input 
 
	if(!allocate_string(string_ptr)){ // check whether allocation of string was succesfull
		return lexer_error(string_ptr, ER_INTERNAL);
	}

	if (source == NULL) // check whether source file was set
	{
		return lexer_error(string_ptr, ER_INTERNAL);
	}

	while(true){
		char c = getc(source); // read characters one by one
		switch(current_status){

			// all possible first states
			case(STATE_START):
				if(isspace(c) || c == '\n' || c == '\t'){
					token->token = TYPE_EOF;
				}
				else if(c == '*'){ // *
					token->token = TYPE_MUL;
					printf("%s\n", tokens[token->token]);
				}
				else if(c == '+'){ // +
					token->token = TYPE_PLUS;
					printf("%s\n", tokens[token->token]);
				}
				else if(c == '/'){ // /
					token->token = TYPE_DIV;
					printf("%s\n", tokens[token->token]);
				}
				else if(c == '-'){ // -
					token->token = TYPE_MINUS;
					printf("%s\n", tokens[token->token]);
				}
				else if(c == '('){ // (
					token->token = TYPE_LEFT_BRACKET;
					printf("%s\n", tokens[token->token]);
				}
				else if(c == ')'){ // )
					token->token = TYPE_RIGHT_BRACKET;
					printf("%s\n", tokens[token->token]);
				}
				else if(c == ','){ // ,
					token->token = TYPE_COMMA;
					printf("%s\n", tokens[token->token]);
				}
				else if(c == '<'){ // <
					change_state(&current_status, STATE_LESS_THAN);
				}
				else if(c == '>'){ // >
					change_state(&current_status, STATE_GREATER_THAN);
				}
				else if(c == '='){ // =
					change_state(&current_status, STATE_ASSIGN);
				}
				else if(c == '#'){ // #
					token->token = TYPE_COMMENT;
					change_state(&current_status, STATE_LINE_COMMENTARY);
				}
				else if(c == '!'){ // !
					change_state(&current_status, STATE_EXCLAMATION_MARK);
				}
				else if(c == '"'){ // "
					change_state(&current_status, STATE_STRING_LITERAL);
				}
				else if(isdigit(c)){ // [0-9]
					if(c == '0'){ // number starts with 0, therefore it can be 0 or 0.XXXX, but not 0XXX, X is [1-9]
						if(!add_char(string_ptr, c)){
							return lexer_error(string_ptr, ER_INTERNAL);
						}
						change_state(&current_status, STATE_FIRST_ZERO);
					}
					else{ // first digit is not zero, therefore it can be followed by any other digit including zero
						if(!add_char(string_ptr, c)){
							return lexer_error(string_ptr, ER_INTERNAL);
						}
						change_state(&current_status, STATE_FIRST_NONZERO);
					}
				}
				else if(isalpha(c) || c == '_'){ // [a-zA-Z_]
					if(islower(c) || c == '_'){ // first char is [a-z_], so it can be an identifier
						if(!add_char(string_ptr, c)){
							return lexer_error(string_ptr, ER_INTERNAL);
						}
						change_state(&current_status, STATE_NEXT_CHARS);
					}
					else{ // can't begin with [A-Z], lexical error
						return lexer_error(string_ptr, ER_LEX);
					}
				}
				else if(c == EOF){ // should this be here???
					token->token = TYPE_EOF;
					change_state(&current_status, STATE_EOF);
				}
				else{ // non-acceptable char
					return lexer_error(string_ptr, ER_LEX);
				}
				break;

			// previous char was <
			case(STATE_LESS_THAN):
				if(c == '='){ // <=
					token->token = TYPE_LEQ;
					printf("%s\n", tokens[token->token]);
					change_state(&current_status, STATE_START);
				
				}
				else{ // <
					token->token = TYPE_LTN;
					printf("%s\n", tokens[token->token]);
					ungetc(c, source); // puts c back to buffer
					change_state(&current_status, STATE_START); // go to start state
				}
				break;

			// previous char was >
			case(STATE_GREATER_THAN):
				if(c == '='){ // >=
					token->token = TYPE_MEQ;
					printf("%s\n", tokens[token->token]);
					change_state(&current_status, STATE_START);

				}
				else{ // >
					token->token = TYPE_MTN;
					printf("%s\n", tokens[token->token]);
					ungetc(c, source); // puts c back to buffer
					change_state(&current_status, STATE_START); // go to start state
				}
				break;

			// previous char was =
			case(STATE_ASSIGN):
				if(c == '='){ // ==
					token->token = TYPE_EQ;
					printf("%s\n", tokens[token->token]);
					change_state(&current_status, STATE_START);
				}
				else if(isalpha(c) && c == 'b'){ // =b; expesting it to be '=begin'
					if(!add_char(string_ptr, '=')){
						return lexer_error(string_ptr, ER_INTERNAL);
					}
					if(!add_char(string_ptr, c)){
						return lexer_error(string_ptr, ER_INTERNAL);
					} // use dynamic string, store b
					change_state(&current_status, STATE_COMMENT_START);	// go to state that checks whether it is start of block comment
				}
				else{ // =
					token->token = TYPE_ASSIGN;
					printf("%s\n", tokens[token->token]);
					ungetc(c, source); // puts c back to buffer
					change_state(&current_status, STATE_START);
				}
				break;

			// line commentary # I am a comment
			case(STATE_LINE_COMMENTARY):
				token->token = TYPE_COMMENT; // the entire line is comment
				if(c == '\n'){ // waits for the end of line char
					printf("LINE COMMENT\n");
					change_state(&current_status, STATE_START);
				}
				break;

			// check whether input is '=begin'
			case(STATE_COMMENT_START):	
				if(!add_char(string_ptr, c)){
					return lexer_error(string_ptr, ER_INTERNAL);
				}
				registered_input = strlen(string_ptr->s);

				if(check_comment_begin(registered_input, string_ptr)){
					if(registered_input == strlen("=begin")){ // it is '=begin'
						printf("STRING: %s : %s\n", string_ptr->s, tokens[token->token]);
						clear_string_content(string_ptr);
						change_state(&current_status, STATE_INSIDE_BLOCK_COMMENT);
					}
				}
				else{
					// Not matching - has to remove the first '=' and save the string ?? TODO
					for(int i = 1; i < registered_input; i++){
						ungetc(string_ptr->s[i], source); // does this work????? I DUNNO
					}
					token->token = TYPE_ASSIGN;
					clear_string_content(string_ptr);
					change_state(&current_status, STATE_START);
				}
				break;

			// waits for =, everything inside the block comment is ignored
			case(STATE_INSIDE_BLOCK_COMMENT):
				if(c == '='){
					if(!add_char(string_ptr, c)){
						return lexer_error(string_ptr, ER_INTERNAL);
					}
					change_state(&current_status, STATE_COMMENT_END);
				}
				else{
					token->token = TYPE_COMMENT;
				}
				break;

			// check whether input is '=end'
			case(STATE_COMMENT_END):
				if(!add_char(string_ptr, c)){
					return lexer_error(string_ptr, ER_INTERNAL);
				}
				registered_input = strlen(string_ptr->s);
				if(check_comment_end(registered_input, string_ptr)){ // it is '=end'
					if(registered_input == strlen("=end")){
						printf("STRING: %s : %s\n", string_ptr->s, tokens[token->token]);
						clear_string_content(string_ptr);
						change_state(&current_status, STATE_START);
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
					return lexer_error(string_ptr, ER_LEX);
				}
				break;
			
			// array of chars
			case(STATE_NEXT_CHARS):
				if(isalpha(c) || isdigit(c) || c == '_'){
					if(!add_char(string_ptr, c)){
						return lexer_error(string_ptr, ER_INTERNAL);
					} // adding new chars to the string
				}
				else if(c == '?' || c == '!'){ // this has to be the end of string
					if(!add_char(string_ptr, c)){
						return lexer_error(string_ptr, ER_INTERNAL);
					}
					change_state(&current_status, STATE_LAST_CHAR);
				}
				else{ // string is complete
					ungetc(c, source); // puts c back to buffer
					keywords(string_ptr, token); // compares it with all keywords
					clear_string_content(string_ptr);
					change_state(&current_status, STATE_START);
				}
				break;

			// last char can be ! or ?, but it cannot be followed by anything else
			case(STATE_LAST_CHAR):
				if(isalpha(c) || isdigit(c) || c == '_'){ // it cannot by followd by anything else
					return lexer_error(string_ptr, ER_LEX);
				}
				else{ // this can be identifier
					ungetc(c, source);
					keywords(string_ptr, token); // not needed, no keywords ends with ? or !
					clear_string_content(string_ptr);
					change_state(&current_status, STATE_START);
				}
				break;

			// string literal enclosed in two double quotation marks
			case(STATE_STRING_LITERAL):
				if(c == '"'){ // this is the end of string literal
					token->token = TYPE_STRING_LITERAL;
					token->attr.string = string_ptr->s;
					printf("STRING: %s : %s\n", string_ptr->s, tokens[token->token]);
					clear_string_content(string_ptr);
					change_state(&current_status, STATE_START);
				}
				else{
					if(c == '\\'){ // special escape sequences
						change_state(&current_status, STATE_BACKSLASH_LITERAL);
					}
					else{ // adds it to the literal string
						if(!add_char(string_ptr, c)){
							return lexer_error(string_ptr, ER_INTERNAL);
						}
					}
				}
				break;

			// backslash in literal string, indicates escape sequence
			case(STATE_BACKSLASH_LITERAL):
				if(c == 'n'){ // end of line
					if(!add_char(string_ptr, '\n')){
						return lexer_error(string_ptr, ER_INTERNAL);
					}
				}
				else if(c == 's'){ // space
					if(!add_char(string_ptr, ' ')){
						return lexer_error(string_ptr, ER_INTERNAL);
					}
				}
				else if(c == '\\'){ // backslash
					if(!add_char(string_ptr, '\\')){
						return lexer_error(string_ptr, ER_INTERNAL);
					}
				}
				else if(c == '"'){ // quotation mark
					if(!add_char(string_ptr, '"')){
						return lexer_error(string_ptr, ER_INTERNAL);
					}
				}
				else if(c == 't'){ // tab
					if(!add_char(string_ptr, '\t')){
						return lexer_error(string_ptr, ER_INTERNAL);
					}
				}
				else if(c == 'x'){ // hexadecimal number
					change_state(&current_status, STATE_HEX_NUM);
					break;
				}
				change_state(&current_status, STATE_STRING_LITERAL);
				break;

			// converts hex number to int, \xhh
			case(STATE_HEX_NUM):
				if(isdigit(c)){ // checks first h
					hex_num[0] = c;
				}
				else if(c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c =='F'){
					hex_num[0] = c;
				}

				c = getc(source); // gets next char

				if(isdigit(c)){ // checks whether it is a hexadecimal digit or not
					hex_num[1] = c;
				}
				else if(c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c =='F'){
					hex_num[1] = c;
				}
				else{
					ungtec(c, source); // it was only a one digit hexadecimal number, returns c to buffer
				}

				char converted = convert_from_hex(hex_num); // converts hexadecimal to int that is displayed as ASCII character
				if(!add_char(string_ptr, converted)){ // adds it to string literal
					return lexer_error(string_ptr, ER_INTERNAL);
				}
				change_state(&current_status, STATE_STRING_LITERAL); // go back to string literal state
				break;

			// first char was 0
			case(STATE_FIRST_ZERO):
				if(c == '.'){ // 0.xxx
					if(!add_char(string_ptr, c)){
						return lexer_error(string_ptr, ER_INTERNAL);
					}
					change_state(&current_status, STATE_DECIMAL);
				}
				else if(isdigit(c)){ // cannot be 0X, X is [1-9]
					return lexer_error(string_ptr, ER_LEX);
				}
				else{ // it is only 0
					ungetc(c, source);
					token->token = TYPE_INT;
					token->attr.integer = 0;
					printf("%s : %d\n", tokens[token->token], token->attr.integer);
					change_state(&current_status, STATE_START);
				}
				break;

			// first character was a non-zero digit
			case(STATE_FIRST_NONZERO):
				if(isdigit(c)){ // saves all digits ans stays in this state
					if(!add_char(string_ptr, c)){
						return lexer_error(string_ptr, ER_INTERNAL);
					}
				}
				else if(c == '.'){ // it will be a decimal number
					if(!add_char(string_ptr, c)){
						return lexer_error(string_ptr, ER_INTERNAL);
					}
					change_state(&current_status, STATE_DECIMAL);
				}
				else if(c == 'e' || c == 'E'){ // it will be written with exponent
					if(!add_char(string_ptr, c)){
						return lexer_error(string_ptr, ER_INTERNAL);
					}
					change_state(&current_status, STATE_EXPONENTIAL_SIGN);
				}
				else{ // it is int, put c back to buffer and save it as an int
					ungetc(c, source);
					token->token = TYPE_INT;
					token->attr.integer = (int) strtol(string_ptr->s, NULL, 10);
					printf("%s : %d\n", tokens[token->token], token->attr.integer);
					clear_string_content(string_ptr);
					change_state(&current_status, STATE_START);
				}
				break;
			
			// previous chars were digits followed by dot
			case(STATE_DECIMAL):
				if(isdigit(c)){
					if(!add_char(string_ptr, c)){
						return lexer_error(string_ptr, ER_INTERNAL);
					}
				}
				else if(c == 'e' || c == 'E'){
					if(!add_char(string_ptr, c)){
						return lexer_error(string_ptr, ER_INTERNAL);
					}
					change_state(&current_status, STATE_EXPONENTIAL_SIGN);
				}
				else{ // it is decimal float, put c back to buffer and save it as float
					ungetc(c, source);
					token->token = TYPE_FLOAT;
					token->attr.flt = strtof(string_ptr->s, NULL);
					printf("%s : %f\n", tokens[token->token], token->attr.flt);
					clear_string_content(string_ptr);
					change_state(&current_status, STATE_START);
				}
				break;

			// chooses the sign of exponential
			case(STATE_EXPONENTIAL_SIGN):
				if(c == '+' || c == '-'){ // non-mandatory sign
					if(!add_char(string_ptr, c)){
						return lexer_error(string_ptr, ER_INTERNAL);
					}
				}
				else if(isdigit(c)){ // if no sign, default sign is '+'
					ungetc(c, source); // put c back to buffer
					if(!add_char(string_ptr, '+')){
						return lexer_error(string_ptr, ER_INTERNAL);
					}
				}
				change_state(&current_status, STATE_EXPONENTIAL); // go to exponential state
				break;

			// number with mantisa and exponent
			case(STATE_EXPONENTIAL):
				if(isdigit(c)){ // adds c 
					if(!add_char(string_ptr, c)){
						return lexer_error(string_ptr, ER_INTERNAL);
					}
				}
				else{ // input is done, put c back to buffer, convert it and save it as float
					ungetc(c, source);
					token->token = TYPE_FLOAT;
					token->attr.flt = strtof(string_ptr->s, NULL);
					printf("%s : %f\n", tokens[token->token], token->attr.flt);	
					clear_string_content(string_ptr);
					change_state(&current_status, STATE_START);
				}
				break;
		}
		// should this be here? I dunno	
		if(c == EOF){
			free_string(string_ptr);
			break;
		}
	}
	return LEXER_OK;
}