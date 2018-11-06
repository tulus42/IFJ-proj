#include "expression.h"

/**
 * 
 * Keď by bolo že a + 3 - func(argvs), 
 * ako prideme na to, že čo vracia func?
 * Jedna sa o rozšírenie FUNEXP
 * 
*/

bool handle_expression(Symbol_stack_t* stack){
    ;
}


void init_stack(Symbol_stack_t* stack){
    stack->top = NULL;
}

bool push_stack(Symbol_item_t* item){
    ;
}

void free_stack(Symbol_stack_t* stack){
    ;
}

Symbol_item_t* get_stack_top(Symbol_stack_t* stack){
    return stack->top;
}