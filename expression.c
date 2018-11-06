#include "expression.h"

#define GET_TOKEN()                                     \
    if (get_next_token(data->token) != LEXER_OK)        \
        return(ER_LEX)

/**
 * 
 * Keď by bolo že a + 3 - func(argvs), 
 * ako prideme na to, že čo vracia func?
 * Jedna sa o rozšírenie FUNEXP
 * 
*/

Symbol_stack_t stack;

int handle_expression(Data_t* data){
    //init_stack(&stack);

    //push_stack(&stack);

    //free_stack(&stack);
    return EXPRESSION_OK;
}
























void init_stack(Symbol_stack_t* stack){
    stack->top = NULL;
}

bool push_stack(Symbol_stack_t* stack){
    Symbol_item_t* new_item = malloc(sizeof(Symbol_item_t));

    if(new_item == NULL){
        return false;
    }
    else{
        new_item->next = stack->top;
        stack->top = new_item;
        return true;
    }
    
}

bool pop_stack(Symbol_item_t* item){
    ;
}

void free_stack(Symbol_stack_t* stack){
    while(stack->top != NULL){
        Symbol_item_t* tmp = stack->top;
        stack->top = tmp->next;
        free(tmp);
    }
}

Symbol_item_t* get_stack_top(Symbol_stack_t* stack){
    return stack->top;
}