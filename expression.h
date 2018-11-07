/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>


#define table_size 9
#define EXPRESSION_OK 0
#define ERR_EXPRESION 1

typedef enum{
    S,  // shift <
    R,  // reduce >
    E,  // equal =
    U   // undefined - no rule, error
} Precedential_table_rule;

int precedential_table[table_size][table_size] = {
 //  +  -  *  /  (  )  i  R  $
    {R, S, S, S, S, R, S, R, R}, // +
    {R, R, S, S, S, R, S, R, R}, // -
    {R, R, R, R, S, R, S, R, R}, // *
    {R, R, R, R, S, R, S, R, R}, // /
    {S, S, S, S, S, E, S, S, U}, // (
    {R, R, R, R, U, R, U, R, R}, // )
    {R, R, R, R, U, R, U, R, R}, // ID
    {S, S, S, S, S, R, S, U, R}, // Relational operators
    {S, S, S, S, S, U, S, S, U}, // $

};

typedef enum{
    INDEX_PLUS,                 // +
    INDEX_MINUS,                // -
    INDEX_MUL,                  // *
    INDEX_DIV,                  // /
    INDEX_LEFT_BRACKET,         // (
    INDEX_RIGHT_BRACKET,        // )
    INDEX_IDENTIFIER,           // ID
    INDEX_RELATIONAL_OPERATION, // Relational operators
    INDEX_DOLLAR                // $
} Precedential_table_index;

typedef enum{
    PLUS,   // +
    MINUS,  // -
    MUL,    // *
    DIV,    // /
    LEFT_B, // (
    RIGHT_B, // )
    ID,     // i
    EQL,    // ==
    NEQ,   // !=
    LEQ,    // >=
    LTN,    // >
    MEQ,    // <=
    MTN,    // <
    DOLLAR,  // $
    NON_TERMINAL,
    START,
    END
} Precedential_table_symbol;

typedef enum{
    INT, // integer
    FLT, // float
    STR, // string
    NIL, // nil
    OTR  // other
} Data_type;

typedef struct one_item{
    int symbol;
    Data_type type;
    struct one_item* next;
} Symbol_item_t;


typedef struct {
    Symbol_item_t *top;
} Symbol_stack_t;


void print_current_stack(Symbol_stack_t* stack);
void print_token(Data_t* data);
void init_stack(Symbol_stack_t* stack);
void free_stack(Symbol_stack_t* stack);

bool push_stack(Symbol_stack_t* stack, Data_type type, Precedential_table_symbol symbol);
bool pop_stack(Symbol_stack_t* stack);
bool check_expected_token(Data_t* data, Token_type next_token);
bool is_nonterm(Precedential_table_symbol symbol);
bool add_after_first_nonterminal(Symbol_stack_t* stack, Data_type type, Precedential_table_symbol symbol);

int expression_error(Symbol_stack_t* stack);

Symbol_item_t* get_stack_top(Symbol_stack_t* stack);

Precedential_table_symbol get_symbol_from_token(Data_t* data);
Precedential_table_symbol get_first_nonterm(Symbol_stack_t* stack);

Precedential_table_rule get_indexes_and_rule(Symbol_stack_t* stack, Data_t* data);

Precedential_table_index get_index(Precedential_table_symbol symbol);

Data_type get_data_type(Data_t* data);

Precedential_table_rule get_rule(Precedential_table_symbol rows, Precedential_table_symbol columns);