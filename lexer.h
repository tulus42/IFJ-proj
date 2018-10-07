/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

#include <stdio.h>

// TODO - Finish the state list, give it proper numbers
#define STATE_START 500 // start
#define STATE_ASSIGN 504 // =
#define STATE_LESS_THAN 506 // <
#define STATE_GREATER_THAN 508 // >
#define STATE_COMMA 514
#define STATE_EXCLAMATION_MARK 515
#define STATE_LINE_COMMENTARY 517
#define STATE_EOL 518
#define STATE_EON 519
#define STATE_NEXT_CHARS 520
#define STATE_FIRST_ZERO 521
#define STATE_FIRST_NONZERO 522
#define STATE_COMMENT_START
#define STATE_COMMENT_END


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

} Keyword;

typedef union
{
	char *string; 
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

	TYPE_LEFT_BRACKET, // (
	TYPE_RIGHT_BRACKET, // )
	TYPE_COMMA, // ,

	TYPE_COMMENT, // #
	TYPE_COMMENT_START, // =begin 
	TYPE_COMMENT_END, // =end 

	TYPE_INT, 
	TYPE_FLOAT, 
	TYPE_STRING,
} Token_type;

typedef struct
{
    Token_type token;
    Token_attr attr;
} Token_t;

void get_source(FILE *f);
int get_next_token(Token_t *token);