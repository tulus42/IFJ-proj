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
#define STATE_EXCLAMATION_MARK 515 // !
#define STATE_LINE_COMMENTARY 517 // # I am a line comment and get ignored
#define STATE_EOL 518
#define STATE_EOF 519
#define STATE_NEXT_CHARS 520
#define STATE_FIRST_ZERO 521
#define STATE_FIRST_NONZERO 522
#define STATE_LAST_CHAR 523 // ! or ?
#define STATE_COMMENT_START 524 // '=begin'
#define STATE_INSIDE_BLOCK_COMMENT 525
#define STATE_COMMENT_END 526 // '=end'
#define STATE_STRING_LITERAL 527 // "xxxxxx"
#define STATE_BACKSLASH_LITERAL 528 // backslash
#define STATE_HEX_NUM 529 // xx
#define STATE_DECIMAL 530 // 5.42

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
	TYPE_STRING_LITERAL, 

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

	TYPE_LEFT_BRACKET, // (
	TYPE_RIGHT_BRACKET, // )
	TYPE_COMMA, // ,

	TYPE_COMMENT, // #
	TYPE_COMMENT_START, // =begin 
	TYPE_COMMENT_END, // =end 

	TYPE_INT, 
	TYPE_FLOAT, 
	TYPE_STRING, /// STRING A STRING LITERAL - ROZDIEL?????
} Token_type;

typedef struct
{
    Token_type token;
    Token_attr attr;
} Token_t;

void get_source(FILE *f);
int get_next_token(Token_t *token);
void change_state(int * current_state, int next_state);
//int lexer_error(struct string_t* string_ptr);