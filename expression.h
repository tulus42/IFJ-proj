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


#include "parser.h"

#ifndef EXPRESSION_H
#define EXPRESSION_H

#define table_size 9
#define EXPRESSION_OK 0
#define OTHER_SYNTACTICAL_ERRORS 2
#define UNDEFINED_ID_EXPRESSION 3




typedef enum{
    S,  // shift <
    R,  // reduce >
    E,  // equal =
    U   // undefined - no rule, error
} Precedential_table_rule;

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
    IDIV
} Precedential_table_symbol;

typedef enum{
    ON_GENERATOR_STACK,
    INVALID_TOKEN,
    VALID_TOKEN
} Symbol_status;

typedef struct one_item{
    int symbol;
    struct one_item* next;
    Tmp_Token_t my_token;
    Symbol_status current_status;
} Symbol_item_t;


typedef struct {
    Symbol_item_t* top;
} Symbol_stack_t;

typedef struct {
    Symbol_item_t* first;
    Symbol_item_t* last;
} Symbol_list;

Symbol_stack_t stack;
Symbol_list buffer;


// DEBUG FUNCTIONS
void print_current_stack(Symbol_stack_t* stack);
void print_token(Data_t* data);
void print_buffer(Symbol_list* list);

/***********************************************************
 * 
 *                  EXPRESSION FUNCTIONS
 * 
 **********************************************************/
void pop_count(int n);
void check_sematics(Data_t* data);

bool allowed_string_operations(Precedential_table_symbol symbol);
bool check_operations(Symbol_stack_t* stack, int to_pop, Precedential_table_symbol operand);
bool add_after_first_terminal(Symbol_stack_t* stack, Precedential_table_symbol symbol);
bool reduce_by_rule(Symbol_stack_t* stack);
bool is_term(Precedential_table_symbol symbol);
bool check_plus(Symbol_stack_t* stack, int to_pop);
bool reduce_identifier(Symbol_item_t* tmp, int count, Symbol_stack_t* stack);
bool reduce_brackets(Symbol_item_t* tmp, int count, Symbol_stack_t* stack, bool preserve_token);
bool push_reduced(int count, Symbol_stack_t* stack);

int insert_stop(Symbol_list* list);
int handle_expression(Data_t* data);
int count_to_reduce(Symbol_stack_t* stack);
int expression_error(Symbol_stack_t* stack, Symbol_list* list, int error_type);
int remember_token(Symbol_item_t* tmp, Data_t* data);

Precedential_table_symbol get_first_term(Symbol_stack_t* stack);
Precedential_table_symbol get_symbol_from_token(Data_t* data);

Precedential_table_rule get_indexes_and_rule(Symbol_stack_t* stack, Precedential_table_symbol symbol);
Precedential_table_rule get_rule(Precedential_table_symbol rows, Precedential_table_symbol columns);

Precedential_table_index get_index(Precedential_table_symbol symbol);

/***********************************************************
 * 
 *                  STACK FUNCTIONS
 * 
 **********************************************************/
void init_stack(Symbol_stack_t* stack);
void free_stack(Symbol_stack_t* stack);

bool push_stack(Symbol_stack_t* stack, Precedential_table_symbol symbol, Data_t* data);
bool push_no_token(Symbol_stack_t* stack, Precedential_table_symbol symbol);
bool pop_stack(Symbol_stack_t* stack);

/***********************************************************
 * 
 *                  BUFFER FUNCTIONS
 * 
 **********************************************************/
void init_buffer(Symbol_list* List);
void clear_buffer(Symbol_list* list);
void delete_first(Symbol_list* list);

bool is_empty(Symbol_list* list);

int insert_to_buffer(Symbol_list* list, Data_t* data);

Symbol_item_t* get_from_buffer(Symbol_list* list);

#endif

