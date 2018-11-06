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

typedef struct{
    int symbol;
    Data_type type;
    struct Symbol_item_t* next;
} Symbol_item_t;


typedef struct {
    Symbol_item_t *top;
} Symbol_stack_t;