/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

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

/**
 * Handles the epxression, get the first token an token that symbolizes end of expression 
 */

int handle_expression(Data_t* data, Token_type next_token){
    bool go_back = false;
    Data_type to_push_type;
    Precedential_table_symbol to_push_symbol;
    Precedential_table_rule current_rule;

    init_stack(&stack); // initialize stack

    /*
    while(!check_expected_token(data, next_token)){
        GET_TOKEN();
        print_token(data);
    }
    */
    
    if(!push_stack(&stack, OTR, DOLLAR)){ // push dollar
        return expression_error(&stack);
    } 
    
    while(!check_expected_token(data, next_token)){
        GET_TOKEN();
        print_token(data);
        current_rule = get_indexes_and_rule(&stack, data);
        
        
        switch(current_rule){
            case(S):
                to_push_type = get_data_type(data);
                to_push_symbol = get_symbol_from_token(data);
                
                if(!push_stack(&stack, OTR, START)){ // push start symbol
                    return expression_error(&stack);
                }
                if(!push_stack(&stack, to_push_type, to_push_symbol)){ // push token symbol
                    return expression_error(&stack);
                }
                //break;
            case(R):
                //break;
            case(E):
                //break;
            case(U):
                return expression_error(&stack);
                //break;
        }
        //print_current_stack(&stack);
        
    }
    
    
    

    free_stack(&stack);
    return EXPRESSION_OK;
}

/**
 * 
 */
Precedential_table_rule get_indexes_and_rule(Symbol_stack_t* stack, Data_t* data){
    int rows = get_index(stack->top->symbol);
    int columns = get_index(get_symbol_from_token(data));
    return get_rule(rows, columns);
}

/**
 * 
 */
Data_type get_data_type(Data_t* data){
    if(data->token->token == TYPE_INT)
        return INT;
    else if(data->token->token == TYPE_FLOAT)
        return FLT;
    else if(data->token->token == TYPE_STRING)
        return STR;
    else
        return OTR;
}

/**
 * Get rule according to columns and rows
 */
Precedential_table_rule get_rule(Precedential_table_symbol rows, Precedential_table_symbol columns){
    return precedential_table[rows][columns];
}

/**
 * 
 */
int expression_error(Symbol_stack_t* stack){
    free_stack(stack);
    return ERR_EXPRESION;
}

/**
 * 
 */
void print_token(Data_t* data){
    if(data->token->token)
        printf("%s\n", tokens[data->token->token]);
}

/**
 * 
 */
void print_current_stack(Symbol_stack_t* stack){ // 
    if(stack->top == NULL)
        return;
    else{
        Symbol_item_t* tmp = stack->top;
        int i = 0;
        while(tmp != NULL){
            printf("%d : %s\n", i, symbols[tmp->symbol]);
            i++;
            tmp = tmp->next;
        }
        printf("\n");
    }
}

/**
 * Returns symbol of precedential table from token 
 */
Precedential_table_symbol get_symbol_from_token(Data_t* data){
    switch(data->token->token){
        case(TYPE_PLUS):
            return PLUS;
        case(TYPE_MINUS):
            return MINUS;
        case(TYPE_MUL):
            return MUL;
        case(TYPE_DIV):
            return DIV;
        case(TYPE_LEFT_BRACKET):
            return LEFT_B;
        case(TYPE_RIGHT_BRACKET):
            return RIGHT_B;
        case(TYPE_INT):
        case(TYPE_STRING):
        case(TYPE_FLOAT):
        case(TYPE_IDENTIFIER):
            return ID;
        case(TYPE_EQ):
            return EQL;
        case(TYPE_NEQ):
            return NEQ;
        case(TYPE_LEQ):
            return LEQ;
        case(TYPE_LTN):
            return LTN;
        case(TYPE_MEQ):
            return MEQ;
        case(TYPE_MTN):
            return MTN;
        default:
            return DOLLAR;
    }
}

/**
 * Gets index number from symbol
 */
Precedential_table_index get_index(Precedential_table_symbol symbol){
    switch(symbol){
        case(PLUS):
            return INDEX_PLUS;
        case(MINUS):
            return INDEX_MINUS;
        case(MUL):
            return INDEX_MUL;
        case(DIV):
            return INDEX_DIV;
        case(LEFT_B):
            return INDEX_LEFT_BRACKET;
        case(RIGHT_B):
            return INDEX_RIGHT_BRACKET;
        case(ID):
            return INDEX_IDENTIFIER;
        case(EQL):
        case(NEQ):
        case(LEQ):
        case(LTN):
        case(MEQ):
        case(MTN):
            return INDEX_RELATIONAL_OPERATION;
        case(DOLLAR):
            return INDEX_DOLLAR;
    }
}



/**
 * If we got the expected token, return true, else false
 */
bool check_expected_token(Data_t* data, Token_type next_token){
    if(next_token == TYPE_EOL_OR_EOF){
        if(data->token->token == TYPE_EOF)
            return true;
        else if(data->token->token == TYPE_EOL)
            return true;
    }
    else if(data->token->token == TYPE_KEYWORD){
        if(data->token->attr.keyword == next_token){
            return true;
        }
        return false;
    }
    return false;
}

/**
 * Initializes top of stack to null
 */
void init_stack(Symbol_stack_t* stack){
    stack->top = NULL;
}

/**
 * Pushes symbol to stack
 */
bool push_stack(Symbol_stack_t* stack, Data_type type, Precedential_table_symbol symbol){
    Symbol_item_t* new_item = malloc(sizeof(Symbol_item_t));

    if(new_item == NULL){
        return false;
    }
    else{
        new_item->next = stack->top;
        new_item->symbol = symbol;
        new_item->type = type;
        stack->top = new_item;
        return true;
    }  
}

/**
 * Pops symbol from stack
 */
bool pop_stack(Symbol_stack_t* stack){
    Symbol_item_t* tmp = stack->top;
    if(tmp == NULL){
        return true;
    }
    else if(tmp->next == NULL){
        free(tmp);
    }
    else{
        stack->top = stack->top->next;
        free(tmp);
        tmp = NULL;
    }
    return true;
}

/**
 * Frees all the memory of stack - used at the end of expression or when error
 */
void free_stack(Symbol_stack_t* stack){
    if(stack->top == NULL){
        return;
    }
    while(stack->top != NULL){
        Symbol_item_t* tmp = stack->top;
        stack->top = tmp->next;
        free(tmp);
    }
}

Symbol_item_t* get_stack_top(Symbol_stack_t* stack){
    return stack->top;
}