/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

#include <stdio.h>


#include "dynamic_string.h"

#ifndef LEXER
#define LEXER



#define LEXER_OK 0

// add keywords
// TODO - Finish the state list, give it proper numbers
#define STATE_START 500 // start
#define STATE_ASSIGN 501 // =
#define STATE_LESS_THAN 502 // <
#define STATE_GREATER_THAN 503 // >
#define STATE_COMMA 504
#define STATE_EXCLAMATION_MARK 505 // !
#define STATE_LINE_COMMENTARY 506 // # I am a line comment and get ignored
#define STATE_EOL 507
#define STATE_EOF 508
#define STATE_NEXT_CHARS 509
#define STATE_FIRST_ZERO 510
#define STATE_FIRST_NONZERO 511
#define STATE_LAST_CHAR 512 // ! or ?
#define STATE_COMMENT_START 513 // '=begin'
#define STATE_INSIDE_BLOCK_COMMENT 514
#define STATE_COMMENT_END 515 // '=end'
#define STATE_STRING 516 // "xxxxxx"
#define STATE_BACKSLASH_LITERAL 517 // backslash
#define STATE_HEX_NUM 518 // xx
#define STATE_DECIMAL 519 // 5.42
#define STATE_EXPONENTIAL 520
#define STATE_EXPONENTIAL_SIGN 521
#define STATE_BINARY_NUM 522
#define STATE_OCTAL_NUM 523
#define STATE_HEXADECIMAL_NUM 525
#define STATE_EXPECT_COMMENT 526
#define STATE_EXPECT_NEWLINE 527
#define STATE_INVALID_END 528

typedef enum
{
	KEYWORD_DEF,
	KEYWORD_DO,
	KEYWORD_ELSE,
	KEYWORD_END,
	KEYWORD_IF,
	KEYWORD_NOT,
	KEYWORD_NIL,
	KEYWORD_THEN,
	KEYWORD_WHILE,
	KEYWORD_PRINT,
	KEYWORD_INPUTS,
	KEYWORD_INPUTI,
	KEYWORD_INPUTF,
	KEYWORD_LENGTH,
	KEYWORD_SUBSTR,
	KEYWORD_ORD,
	KEYWORD_CHR,

} Keyword;

typedef union
{
	string_t* string; 
	int integer; 
	Keyword keyword; 
	double flt; // float
} Token_attr;

typedef enum
{
	TYPE_EOF, 
	TYPE_EOL, 
	TYPE_IDENTIFIER, 
	TYPE_KEYWORD,

	TYPE_ASSIGN, // =
	TYPE_NEQ, // !=
	TYPE_LEQ, // <=
	TYPE_LTN, // <
	TYPE_MEQ, // >=
	TYPE_MTN, // >
	TYPE_EQ, // ==
	
	TYPE_PLUS, // +
	TYPE_MINUS, //  -
	TYPE_MUL, // *
	TYPE_DIV, // /
	TYPE_QUESTION_MARK, // ?
	TYPE_COLON, // :

	TYPE_LEFT_BRACKET, // (
	TYPE_RIGHT_BRACKET, // )
	TYPE_COMMA, // ,

	TYPE_COMMENT, // #
	TYPE_COMMENT_START, // =begin 
	TYPE_COMMENT_END, // =end 

	TYPE_INT, 
	TYPE_FLOAT, 
	TYPE_STRING /// STRING LITERAL
} Token_type;

typedef struct
{
    Token_type token;
    Token_attr attr;
} Token_t;

typedef union{
	char* tmp_string;
	int tmp_integer; 
	Keyword tmp_keyword; 
	double tmp_flt; // float
} Tmp_token_attr_t;

typedef struct
{
	Token_type type_token;
	Tmp_token_attr_t attr_token;
} Tmp_Token_t;

int lexer_error(string_t* string_ptr, int error_type);
int lexer_succesful(string_t* string_ptr);
int get_next_token(Token_t *token);
void keywords(string_t *string_ptr, Token_t* token);
void change_state(int * current_state, int next_state);
void get_source(FILE *f);
void set_string(string_t* string);

#endif

