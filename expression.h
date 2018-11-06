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
    PLUS,   // +
    MINUS,  // -
    MUL,    // *
    DIV,    // /
    EQL,    // ==
    NEQL,   // !=
    LEFT_B, // (
    RIGHT_B, // )

} Precedential_table_symbol;

typedef enum{
    INT, // integer
    FLT, // float
    STR, // string
    NIL // nil
} Data_type;

typedef struct one_item{
    int symbol;
    Data_type type;
    struct one_item* next;
} Symbol_item_t;


typedef struct {
    Symbol_item_t *top;
} Symbol_stack_t;

void init_stack(Symbol_stack_t* stack);
bool push_stack(Symbol_stack_t* stack);
void free_stack(Symbol_stack_t* stack);
Symbol_item_t* get_stack_top(Symbol_stack_t* stack);
bool pop_stack(Symbol_item_t* item);