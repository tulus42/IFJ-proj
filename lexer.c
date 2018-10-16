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

/*
TODO: 
kde je malloc, treba tam return type bool;
return v každom case?
=begin if not match
skontrolovať rozsah int a double?
check how ungetc work when pushing more chars to the buffer
REMOVE ALL THE DEBUG PRINTS!!!!
*/

int lexer_error(struct string_t* string_ptr){
	free_string(string_ptr);
	printf("Lexer ERROR\n");
	return ER_LEX;
}

int get_next_token(Token_t *token) // konecny automat, v podstate while cyklus, ve kterem je switch, nacitame znaky, jakmile urcime token tak ho vratime nebo najdeme blbost a vratime ER_LEX
{
	// current state is start
	int current_status = STATE_START;
	int registered_input;
	// variable for string struct
	struct string_t string;
	struct string_t *string_ptr = &string;
	char hex_num[2] = {'\0', '\0'};

	if(!allocate_string(string_ptr)){
		fprintf(stderr, "ERROR: Allocation of string failed\n");
		return ER_INTERNAL;
	}

	if (source == NULL)
	{
		return ER_INTERNAL;
	}

	while(true){
		char c = getc(source); // read characters one by one
		switch(current_status){
			// all possible first states
			case(STATE_START):
				if(isspace(c) || c == '\n' || c == '\t'){
					token->token = TYPE_EOF;
					//printf("I am whitespace\n");
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
					if(c == '0'){
						;//change_state(&current_status, STATE_FIRST_ZERO);
					}
					else{
						;//change_state(&current_status, STATE_FIRST_NONZERO);
					}
				}
				else if(isalpha(c) || c == '_'){ // [a-zA-Z_]
					if(islower(c) || c == '_'){ // [a-z_]
						// can continue, first char is OK
						add_char(string_ptr, c);
						change_state(&current_status, STATE_NEXT_CHARS);
					}
					else{ // can't begin with uppercase letter, [A-Z]
						return lexer_error(string_ptr);
					}
				}
				else if(c == EOF){
					token->token = TYPE_EOF;
					change_state(&current_status, STATE_EOF);
				}
				else{ // non-acceptable char
					return lexer_error(string_ptr);
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
					add_char(string_ptr, '='); // use dynamic string, store =
					add_char(string_ptr, c); // use dynamic string, store b
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
				add_char(string_ptr, c);
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
					return lexer_error(string_ptr);
				}
				break;
			
			// array of chars
			case(STATE_NEXT_CHARS):
				if(isalpha(c) || isdigit(c) || c == '_'){
					add_char(string_ptr, c); // adding new chars to the string
				}
				else if(c == '?' || c == '!'){ // this has to be the end of string
					add_char(string_ptr, c);
					change_state(&current_status, STATE_LAST_CHAR);
				}
				else{ // string is complete
					ungetc(c, source);
					//printf("I am %s and my token type is %s\n", string_ptr->s, tokens[token->token]);
					keywords(string_ptr, token);
					clear_string_content(string_ptr);
					change_state(&current_status, STATE_START);
				}
				break;

			// last char can be ! or ?, but it cannot be followed by anything else
			case(STATE_LAST_CHAR):
				if(isalpha(c) || isdigit(c) || c == '_'){ // not allowed
					lexer_error(string_ptr);
				}
				else{ // ALL IS WELL, CAN BE IDENTIFIER
					ungetc(c, source);
					keywords(string_ptr, token);
					clear_string_content(string_ptr);
					change_state(&current_status, STATE_START);
				}
				break;

			// "xxxxx"
			case(STATE_STRING_LITERAL):
				if(c == '"'){
					token->token = TYPE_STRING_LITERAL;
					token->attr.string = string_ptr->s;
					printf("STRING: %s : %s\n", string_ptr->s, tokens[token->token]);
					clear_string_content(string_ptr);
					change_state(&current_status, STATE_START);
				}
				else{
					if(c == '\\'){
						change_state(&current_status, STATE_BACKSLASH_LITERAL);
					}
					else{
						add_char(string_ptr, c);
					}
				}
				break;

			// backslash in literal string
			case(STATE_BACKSLASH_LITERAL):
				if(c == 'n'){
					add_char(string_ptr, '\n');
				}
				else if(c == 's'){
					add_char(string_ptr, ' ');
				}
				else if(c == '\\'){
					add_char(string_ptr, '\\');
				}
				else if(c == '"'){
					add_char(string_ptr, '"');
				}
				else if(c == 't'){
					add_char(string_ptr, '\t');
				}
				else if(c == 'x'){
					change_state(&current_status, STATE_HEX_NUM);
					break;
				}
				change_state(&current_status, STATE_STRING_LITERAL);
				break;

			case(STATE_HEX_NUM):
				hex_num[0] = c;
				c = getc(source);
				hex_num[1] = c;

				char converted = convert_from_hex(hex_num);
				add_char(string_ptr, converted);

				change_state(&current_status, STATE_STRING_LITERAL);
				break;

			// this should work like a string
			case(STATE_FIRST_ZERO):
				//token->token = TYPE_INT;
				//token->attr.integer = 0;
				change_state(&current_status, STATE_START);
				break;

			
			case(STATE_FIRST_NONZERO):
				change_state(&current_status, STATE_START);
				break;
			
			
		}
		
		/*
		if(string_ptr->s[0] != '\0'){
			printf("I am %s and my token type is %s\n", string_ptr->s, tokens[token->token]);
		}
		else{
			printf("I am %c token type is %s\n", c, tokens[token->token]);
		}
		*/
		

		if(c == EOF){
			free_string(string_ptr);
			break;
		}
	}
	return LEXER_OK;
}