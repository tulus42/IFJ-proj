/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

#define PROG 1
#define STATEMENT 2
#define DECLARE 3
#define PARAMS 4
#define PARAM 5
#define ARGVS 6
#define ARGV 7
#define VALUE 8

#define NOT_FOUND 0

//#include "lexer.h"

int get_rule_num(int nonterminal, Token_t* token);
void get_rule();