#define PROG 1
#define VALUE 2
#define STATEMENT 3
#define VARIABLE 4
#define DECLARE 5
#define UNDEFINED 6

#include "lexer.h"

int get_rule_num(int nonterminal, Token_t* token);
void get_rule();