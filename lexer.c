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
char* string_content;
string_t* dynamic_string;
static int token_counter = 0;

/*
TODO: 
hex_number error
=begin if not match, beginning???
skontrolovať rozsah int a double?
*/

/**
 * Sets the source file
 */
void get_source(FILE *f){
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
void keywords(string_t *string_ptr, Token_t* token){
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
	else if(compare_strings(string_ptr, "print")){
		token->token = TYPE_KEYWORD;
		token->attr.keyword = KEYWORD_PRINT;
	}
	else if(compare_strings(string_ptr, "inputs")){
		token->token = TYPE_KEYWORD;
		token->attr.keyword = KEYWORD_INPUTS;
	}
	else if(compare_strings(string_ptr, "inputi")){
		token->token = TYPE_KEYWORD;
		token->attr.keyword = KEYWORD_INPUTI;
	}
	else if(compare_strings(string_ptr, "inputf")){
		token->token = TYPE_KEYWORD;
		token->attr.keyword = KEYWORD_INPUTF;
	}
	else if(compare_strings(string_ptr, "length")){
		token->token = TYPE_KEYWORD;
		token->attr.keyword = KEYWORD_LENGTH;
	}
	else if(compare_strings(string_ptr, "substr")){
		token->token = TYPE_KEYWORD;
		token->attr.keyword = KEYWORD_SUBSTR;
	}
	else if(compare_strings(string_ptr, "ord")){
		token->token = TYPE_KEYWORD;
		token->attr.keyword = KEYWORD_ORD;
	}
	else if(compare_strings(string_ptr, "chr")){
		token->token = TYPE_KEYWORD;
		token->attr.keyword = KEYWORD_CHR;
	}
	else{
		token->token = TYPE_IDENTIFIER;
		copy_string_content(token->attr.string, string_ptr);
	}
}

/**
 * Writes error to stderr, returns error type
 */
int lexer_error(string_t* string_ptr, int error_type){
	free_string(string_ptr);
	fprintf(stderr, "Lexer ERROR\n");
	return error_type;
}

/**
 * Frees string when succesful
 */
int lexer_succesful(string_t* string_ptr){
	token_counter++;
	free_string(string_ptr);
	return LEXER_OK;
}

/**
 * Sets dynamic string
 */
void set_string(string_t* string){
	dynamic_string = string;
}

/**
 * Finite state machine, it either return a correct token or EL_LEX if there is lexical error
 */
int get_next_token(Token_t *token)
{
	/* Used variables */
	int current_status = STATE_START; // current state is start
	int registered_input; // counts how many chars have been entered for '=begin' and '=end'
	bool end_of_comment = false;
	
	token->attr.string = dynamic_string;
	

	// variable for string struct
	string_t string;
	string_t *string_ptr = &string;
	if(!allocate_string(string_ptr)){ // check whether allocation of string was succesfull
		return lexer_error(string_ptr, ER_INTERNAL);
	}

	char hex_num[2] = {'\0', '\0'}; // help variable for hexadecimal '\xhh' input 
 
	if (source == NULL) // check whether source file was set
	{
		return lexer_error(string_ptr, ER_INTERNAL);
	}

	clear_string_content(token->attr.string);


	while(true){
		char c = getc(source); // read characters one by one
		switch(current_status){

			// all possible first states
			case(STATE_START):
				if(c == ' ' || c == '\t'){
					break; // doesn't return any token
				}
				else if(c == '*'){ // *
					token->token = TYPE_MUL;
					return lexer_succesful(string_ptr);
				}
				else if(c == '+'){ // +
					token->token = TYPE_PLUS;
					return lexer_succesful(string_ptr);
				}
				else if(c == '/'){ // /
					token->token = TYPE_DIV;
					return lexer_succesful(string_ptr);
				}
				else if(c == '-'){ // -
					token->token = TYPE_MINUS;
					return lexer_succesful(string_ptr);
				}
				else if(c == '('){ // (
					token->token = TYPE_LEFT_BRACKET;
					return lexer_succesful(string_ptr);
				}
				else if(c == ')'){ // )
					token->token = TYPE_RIGHT_BRACKET;
					return lexer_succesful(string_ptr);
				}
				else if(c == ','){ // ,
					token->token = TYPE_COMMA;
					return lexer_succesful(string_ptr);
				}
				else if(c == '?'){ // ?
					token->token = TYPE_QUESTION_MARK;
					return lexer_succesful(string_ptr);
				}
				else if(c == ':'){ //
					token->token = TYPE_COLON;
					return lexer_succesful(string_ptr);
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
					change_state(&current_status, STATE_LINE_COMMENTARY);
				}
				else if(c == '!'){ // !
					change_state(&current_status, STATE_EXCLAMATION_MARK);
				}
				else if(c == '"'){ // "
					change_state(&current_status, STATE_STRING);
				}
				else if(c == '\n'){ // end of line token
					change_state(&current_status, STATE_EXPECT_COMMENT);
					//token->token = TYPE_EOL;
					//return lexer_succesful(string_ptr);
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
					return lexer_succesful(string_ptr);
				}
				else{ // non-acceptable char
					printf("Som tu v chare\n");
					return lexer_error(string_ptr, ER_LEX);
				}
				break;

			case(STATE_EXPECT_COMMENT):
				if(c == '='){
					if(!add_char(string_ptr, c)){
							return lexer_error(string_ptr, ER_INTERNAL);
					}
					change_state(&current_status, STATE_COMMENT_START);
				}
				else{
					ungetc(c, source);
					token->token = TYPE_EOL;
					return lexer_succesful(string_ptr);
				}
				break;

			// previous char was <
			case(STATE_LESS_THAN):
				if(c == '='){ // <=
					token->token = TYPE_LEQ;
					return lexer_succesful(string_ptr);
				}
				else{ // <
					token->token = TYPE_LTN;
					ungetc(c, source); // puts c back to buffer
					return lexer_succesful(string_ptr);
				}
				break;

			// previous char was >
			case(STATE_GREATER_THAN):
				if(c == '='){ // >=
					token->token = TYPE_MEQ;
					return lexer_succesful(string_ptr);

				}
				else{ // >
					token->token = TYPE_MTN;
					ungetc(c, source); // puts c back to buffer
					return lexer_succesful(string_ptr);
				}
				break;


			case(STATE_HEXADECIMAL_NUM):
				if(isdigit(c)){	// hexadecimal number can contain digit
					if(!add_char(string_ptr, c)){
						return lexer_error(string_ptr, ER_INTERNAL);
					}
				} // and these uppercase and lowercase letters
				else if(c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F'){
					if(!add_char(string_ptr, c)){
						return lexer_error(string_ptr, ER_INTERNAL);
					}
				}
				else if(c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f'){
					if(!add_char(string_ptr, c)){
						return lexer_error(string_ptr, ER_INTERNAL);
					}
				}
				else{
					ungetc(c, source);
					int hexadecimal_number = (int) strtol(string_ptr->s, NULL, 16);
					token->token = TYPE_INT;
					token->attr.integer = hexadecimal_number;
					return lexer_succesful(string_ptr);
				}
				break;

			// previous char was =
			case(STATE_ASSIGN):
				if(c == '='){ // ==
					token->token = TYPE_EQ;
					return lexer_succesful(string_ptr);
				}
				else if(isalpha(c) && c == 'b'){ // =b; expesting it to be '=begin'
					if(token_counter == 0){
						if(!add_char(string_ptr, '=')){
							return lexer_error(string_ptr, ER_INTERNAL);
						}
						if(!add_char(string_ptr, c)){
							return lexer_error(string_ptr, ER_INTERNAL);
						} // use dynamic string, store b
						change_state(&current_status, STATE_COMMENT_START);	// go to state that checks whether it is start of block comment
					}
					else{
						return lexer_error(string_ptr, ER_LEX);
					}
				}
				else{ // =
					token->token = TYPE_ASSIGN;
					ungetc(c, source); // puts c back to buffer
					return lexer_succesful(string_ptr);
				}
				break;

			// line commentary # I am a comment
			case(STATE_LINE_COMMENTARY):
				if(c == '\n' || c == EOF){ // waits for the end of line char
					ungetc(c, source);
					token->token = TYPE_COMMENT; // the entire line is comment
					return lexer_succesful(string_ptr);
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
						end_of_comment = false;
						change_state(&current_status, STATE_EXPECT_NEWLINE);
					}
				}
				else{
					return lexer_error(string_ptr, ER_LEX);
				}
				break;

			case(STATE_INVALID_END):
				if(!add_char(string_ptr, c)){
						return lexer_error(string_ptr, ER_INTERNAL);
				}
				registered_input = strlen(string_ptr->s);
				if(check_comment_end(registered_input, string_ptr)){ // it is '=end'
					if(registered_input == strlen("=end")){
						end_of_comment = true;
						return lexer_error(string_ptr, ER_LEX);
					}
				}
				else{
					change_state(&current_status, STATE_INSIDE_BLOCK_COMMENT);
				}
				break;


			// waits for =, everything inside the block comment is ignored
			case(STATE_INSIDE_BLOCK_COMMENT):
				if(c == '='){
					clear_string_content(string_ptr);
					ungetc(c, source);
					change_state(&current_status, STATE_INVALID_END);	
				}
				else if(c == '\n'){
					clear_string_content(string_ptr);
					change_state(&current_status, STATE_COMMENT_END);
				}
				break;

			case(STATE_EXPECT_NEWLINE):
				if(end_of_comment){
					if(c == ' ' || c == '\n' || c == EOF){
						ungetc(c, source);
						token->token = TYPE_COMMENT;
						return lexer_succesful(string_ptr);
					}
				}
				else{
					if(c == '\n'){
						change_state(&current_status, STATE_INSIDE_BLOCK_COMMENT);
					}
					else{
						return lexer_error(string_ptr, ER_LEX);
					}
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
						end_of_comment = true;
						change_state(&current_status, STATE_EXPECT_NEWLINE);
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
					return lexer_succesful(string_ptr);
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
					return lexer_succesful(string_ptr);
				}
				break;

			// last char can be ! or ?, but it cannot be followed by anything else
			case(STATE_LAST_CHAR):
				if(isalpha(c) || isdigit(c) || c == '_'){ // it cannot by followd by anything else
					return lexer_error(string_ptr, ER_LEX);
				}
				else{ // this can be identifier
					ungetc(c, source);
					token->token = TYPE_FUNC;
					copy_string_content(token->attr.string, string_ptr);
					return lexer_succesful(string_ptr);
				}
				break;

			// string literal enclosed in two double quotation marks
			case(STATE_STRING):
				if(c == '"'){ // this is the end of string literal
					token->token = TYPE_STRING;
					copy_string_content(token->attr.string, string_ptr);
					return lexer_succesful(string_ptr);
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
				change_state(&current_status, STATE_STRING);
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
					ungetc(c, source); // it was only a one digit hexadecimal number, returns c to buffer
				}

				char converted = convert_from_hex(hex_num); // converts hexadecimal to int that is displayed as ASCII character
				if(!add_char(string_ptr, converted)){ // adds it to string literal
					return lexer_error(string_ptr, ER_INTERNAL);
				}
				change_state(&current_status, STATE_STRING); // go back to string literal state
				break;

			// first char was 0
			case(STATE_FIRST_ZERO):
				if(c == '.'){ // 0.xxx
					if(!add_char(string_ptr, c)){
						return lexer_error(string_ptr, ER_INTERNAL);
					}
					change_state(&current_status, STATE_DECIMAL);
				}
				else if(c == 'b'){
					change_state(&current_status, STATE_BINARY_NUM);
					clear_string_content(string_ptr);
				}
				else if(c == 'x'){
					change_state(&current_status, STATE_HEXADECIMAL_NUM);
				}
				else if(isdigit(c)){ // 0X, octal number, X is [1-9]
					ungetc(c, source);
					change_state(&current_status, STATE_OCTAL_NUM);
				}
				else{ // it is only 0
					ungetc(c, source);
					token->token = TYPE_INT;
					token->attr.integer = 0;
					return lexer_succesful(string_ptr);
				}
				break;

			// it is binary number
			case(STATE_BINARY_NUM):
				if(c == '1' || c == '0'){
					if(!add_char(string_ptr, c)){
						return lexer_error(string_ptr, ER_INTERNAL);
					}
				}
				else{
					ungetc(c, source);
					int binary = (int) strtol(string_ptr->s, NULL, 2);
					token->token = TYPE_INT;
					token->attr.integer = binary;
					return lexer_succesful(string_ptr);
				}
				break;

			// it is octal number
			case(STATE_OCTAL_NUM):
				if(c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7'){
					if(!add_char(string_ptr, c)){
						return lexer_error(string_ptr, ER_INTERNAL);
					}
				}
				else{
					ungetc(c, source);
					int hex = (int) strtol(string_ptr->s, NULL, 8);
					token->token = TYPE_INT;
					token->attr.integer = hex;
					return lexer_succesful(string_ptr);
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
					return lexer_succesful(string_ptr);
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
					return lexer_succesful(string_ptr);
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
					return lexer_succesful(string_ptr);
				}
				break;
		}
	}
}