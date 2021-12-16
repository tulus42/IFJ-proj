/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/


#include "error.h"
#include "symtable.h"
#include "expression.h"
#include "instructions.h"

/***********************************************************
 * 
 *                  PRECEDENTIAL TABLE
 * 
 **********************************************************/

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


bool from_lexer = false;
bool from_buffer = false;

/***********************************************************
 * 
 *                       MACROS
 * 
 **********************************************************/

/**
 * Gets token from source file
 */
#define GET_TOKEN()                                                             \
    if (get_next_token(data->token) != LEXER_OK)                                \
        return(ER_LEX)


/**
 * Saves symbol to variable
 */
#define SAVE_SYMBOL()                                                           \
    do{                                                                         \
        tmp = get_from_buffer(&buffer);                                         \
        to_push_symbol = tmp->symbol;                                           \
    }while(0);                                                                  \

/**
 * Saves token to push symbol
 */
#define SAVE_TOKEN()                                                            \
    do{                                                                         \
        to_push_symbol = get_symbol_from_token(data);                           \
    }while(0);                                                                  \

/**
 * Gets symbol either from buffer or asks for new token from file 
 */
#define GET_SYMBOL()                                  \
    if(is_empty(&buffer)){  \
        GET_TOKEN();\
        SAVE_TOKEN();\
        from_lexer = true;\
        from_buffer = false;\
    }\
    else{\
        SAVE_SYMBOL();\
        from_lexer = false;\
        from_buffer = true;\
    }   \
    if(return_code != EXPRESSION_OK)                                        \
            return expression_error(&stack, &buffer, return_code);              \


/**
 * Perform an opperation
 */
#define GENERATE_EXPRESSION() \
    gen_stackop(operand);        \


bool finished = false;
bool used_buffer = false;
int return_code = EXPRESSION_OK;

/***********************************************************
 * 
 *                  "main" of expression.c
 * 
 **********************************************************/

/**
 * Handles the epxression, get the first token an token that symbolizes end of expression 
 */
int handle_expression(Data_t* data){
    bool can_get_token = true;
    bool is_reduced = false;
    Precedential_table_symbol to_push_symbol;
    Precedential_table_rule current_rule;
    Symbol_item_t* tmp = NULL;

    if(!push_no_token(&stack, DOLLAR)){
        return expression_error(&stack, &buffer, ER_INTERNAL);
    }

    GET_SYMBOL();


    while(!is_reduced){
        current_rule = get_indexes_and_rule(&stack, to_push_symbol);  // get current rule

        if(current_rule == S){  // SHIFT rule
            can_get_token = true;
            if(!add_after_first_terminal(&stack, START)){      // push start symbol after first terminal
                return expression_error(&stack, &buffer, ER_INTERNAL);
            }
            if(!push_stack(&stack, to_push_symbol, data)){ // push token symbol
                return expression_error(&stack, &buffer, ER_INTERNAL);
            }

        }
        else if(current_rule == R){  // REDUCE rule
            can_get_token = false;                                      // don't get any new token
            if(!reduce_by_rule(&stack)){                                // reduce it
                return expression_error(&stack, &buffer, return_code);
            } 
        }
        else if(current_rule == E){  // EQUAL rule
            can_get_token = true;                                   // can get new token
            if(!push_stack(&stack, to_push_symbol, data)){          // push token symbol
                return expression_error(&stack, &buffer, ER_INTERNAL);
            }
        }
        else if(current_rule == U){  // UNDEFINED rule
            Precedential_table_symbol last_symbol = get_first_term(&stack);
            if(is_empty(&buffer)){
                to_push_symbol = get_symbol_from_token(data);
                if(finished && last_symbol == DOLLAR && to_push_symbol == DOLLAR){ // we are at the and reduced everything succesfully
                    break;
                }
                else{
                    return expression_error(&stack, &buffer, OTHER_SYNTACTICAL_ERRORS);
                }
            }
            else if(!is_empty(&buffer)){
                if(finished && last_symbol == DOLLAR && buffer.first->symbol == DOLLAR){
                    break;
                }
                else{
                    return expression_error(&stack, &buffer, OTHER_SYNTACTICAL_ERRORS);
                }
            }
            else{
                return expression_error(&stack, &buffer, OTHER_SYNTACTICAL_ERRORS);
            }    
        }

        if(can_get_token){
            GET_SYMBOL();
        }   
    }
    // pop the generator stack - that is our expression
    gen_save_expr_res();

    // clearing all
    free_stack(&stack);
    clear_buffer(&buffer);
    return EXPRESSION_OK;
}

/**
 * Pushes reduced expression like this : E x E
 * We do not have to keep the token anymore as it is already 
 * on generator stack
 */
bool push_reduced(int count, Symbol_stack_t* stack){
    Symbol_item_t* new_thing = malloc(sizeof(Symbol_item_t));

    if(new_thing == NULL){
        return false;
    }
    pop_count(count);

    new_thing->next = stack->top;
    new_thing->symbol = NON_TERMINAL;
    new_thing->current_status = ON_GENERATOR_STACK;
    new_thing->my_token.type_token = TYPE_EOL;
    new_thing->in_def = 0;
    stack->top = new_thing;
    return true;
}

/**
 * Reduces expression in brackets and copies it's current status
 */
bool reduce_brackets(Symbol_item_t* tmp, int count, Symbol_stack_t* stack, bool preserve_token){
    Symbol_item_t* new_thing = malloc(sizeof(Symbol_item_t));
    int length;

    if(new_thing == NULL){
        return false;
    }
    new_thing->current_status = tmp->current_status;

    if(preserve_token){
        new_thing->current_status = tmp->current_status;
        new_thing->my_token.type_token = tmp->my_token.type_token;                          
        new_thing->my_token.attr_token.tmp_flt = tmp->my_token.attr_token.tmp_flt;               
        new_thing->my_token.attr_token.tmp_integer = tmp->my_token.attr_token.tmp_integer;       
        new_thing->my_token.attr_token.tmp_keyword = tmp->my_token.attr_token.tmp_keyword;
        new_thing->in_def = tmp->in_def;       
        if(new_thing->my_token.type_token == TYPE_STRING || new_thing->my_token.type_token == TYPE_IDENTIFIER){
            length = strlen(tmp->my_token.attr_token.tmp_string) + 2;
            new_thing->my_token.attr_token.tmp_string = (char *) malloc(length);
            if(new_thing->my_token.attr_token.tmp_string == NULL){
                return ER_INTERNAL;
            } 
            strcpy(new_thing->my_token.attr_token.tmp_string, tmp->my_token.attr_token.tmp_string);
        }
    }
    else{
        new_thing->my_token.type_token = TYPE_EOL;
    }
    // we pop it
    pop_count(count);

    // then add it
    new_thing->next = stack->top;
    new_thing->symbol = NON_TERMINAL;
    stack->top = new_thing;

    return true;
}

/**
 * Function for pushing in nil described as '()'
 */
bool push_nil(int to_pop, Symbol_stack_t* stack){
    Symbol_item_t* new_thing = malloc(sizeof(Symbol_item_t));

    if(new_thing == NULL){
        return false;
    }

    new_thing->current_status = ON_GENERATOR_STACK;
    new_thing->my_token.type_token = TYPE_KEYWORD;
    new_thing->my_token.attr_token.tmp_keyword = KEYWORD_NIL;
    new_thing->symbol = NON_TERMINAL;
    new_thing->in_def = 0;

    gen_push(new_thing->my_token);

    pop_count(to_pop);

    new_thing->next = stack->top;
    stack->top = new_thing;

    return true;
}

/**
 * Reduces identifier from ID to NON-TERMINAL
 */
bool reduce_identifier(Symbol_item_t* tmp, int count, Symbol_stack_t* stack){
    Symbol_item_t* new_thing = malloc(sizeof(Symbol_item_t));
    int length;

    if(new_thing == NULL){
        return false;
    }

    new_thing->current_status = tmp->current_status;
    new_thing->my_token.type_token = tmp->my_token.type_token;                          
    new_thing->my_token.attr_token.tmp_flt = tmp->my_token.attr_token.tmp_flt;               
    new_thing->my_token.attr_token.tmp_integer = tmp->my_token.attr_token.tmp_integer;       
    new_thing->my_token.attr_token.tmp_keyword = tmp->my_token.attr_token.tmp_keyword; 
    new_thing->in_def = tmp->in_def;      
    if(new_thing->my_token.type_token == TYPE_STRING || new_thing->my_token.type_token == TYPE_IDENTIFIER){
        length = strlen(tmp->my_token.attr_token.tmp_string) + 2;
        new_thing->my_token.attr_token.tmp_string = (char *) malloc(length);
            if(new_thing->my_token.attr_token.tmp_string == NULL){
                return ER_INTERNAL;
            } 
        strcpy(new_thing->my_token.attr_token.tmp_string, tmp->my_token.attr_token.tmp_string);
    }

    // we pop it
    pop_count(count);

    // then add it
    new_thing->next = stack->top;
    new_thing->symbol = NON_TERMINAL;
    stack->top = new_thing;

    return true;
}

/**
 * Reduce it by the rule, push to the generetator and carry out the appropriate opperation
 */
bool reduce_by_rule(Symbol_stack_t* stack){
    int count = count_to_reduce(stack);     // how many symbols are we reducing
    int to_pop = count + 1;                 // we have to pop the count + the start symbol
    Symbol_item_t* tmp_first = stack->top;  // first item from the stack
    Precedential_table_symbol operand;
    bool preserve_token = false;

    // we are reducing 1 symbol
    if(count == 1){ // i -> E
        if(tmp_first->symbol == ID){
            if(tmp_first->current_status == VALID_TOKEN){
                gen_push(tmp_first->my_token);
            }
            tmp_first->current_status = ON_GENERATOR_STACK;
            reduce_identifier(tmp_first, to_pop, stack);
        }
        else{
            return_code = OTHER_SYNTACTICAL_ERRORS;
            return false;
        }
    } 
    else if(count == 2){ // we are reducing () as nil
        if(tmp_first->my_token.type_token == TYPE_RIGHT_BRACKET && stack->top->next->my_token.type_token == TYPE_LEFT_BRACKET){
            push_nil(to_pop, stack);
        }
        else{
            return_code = OTHER_SYNTACTICAL_ERRORS;
            return false;
        }
        
    }
    else if(count == 3){ // we are reducing 3 symbols
        Symbol_item_t* tmp_second = stack->top->next;
        Symbol_item_t* tmp_third = stack->top->next->next;

        // first and third symbols are nonterminals, so we just check the second operand to make sure the grammar is correct
        if(tmp_first->symbol == NON_TERMINAL && tmp_third->symbol == NON_TERMINAL){ // E X E
            if(tmp_first->current_status == VALID_TOKEN){
                gen_push(tmp_first->my_token);
                // push it to generetar
            }

            if(tmp_third->current_status == VALID_TOKEN){
                gen_push(tmp_first->my_token);
                // push it to generator
            }

            operand = tmp_second->symbol;
            if(operand == PLUS){         // E + E
                GENERATE_EXPRESSION();
                push_reduced(to_pop, stack);
            }
            else if(operand == MINUS){   // E - E
                GENERATE_EXPRESSION();
                push_reduced(to_pop, stack);
            }
            else if(operand == MUL){     // E * E
                GENERATE_EXPRESSION();
                push_reduced(to_pop, stack);
            }
            else if(operand == DIV){     // E / E
                GENERATE_EXPRESSION();
                push_reduced(to_pop, stack);
            }
            else if(operand == EQL){     // E == E
                GENERATE_EXPRESSION();
                push_reduced(to_pop, stack);
            }
            else if(operand == NEQ){     // E != E
                GENERATE_EXPRESSION();
                push_reduced(to_pop, stack);
            }
            else if(operand == LEQ){     // E <= E
                GENERATE_EXPRESSION();
                push_reduced(to_pop, stack);
            }
            else if(operand == LTN){     // E < E
                GENERATE_EXPRESSION();
                push_reduced(to_pop, stack);
            }
            else if(operand == MEQ){     // E >= E
                GENERATE_EXPRESSION();
                push_reduced(to_pop, stack);
            }
            else if(operand == MTN){     // E > E
                GENERATE_EXPRESSION();
                push_reduced(to_pop, stack);
            }
            else{
                return_code = OTHER_SYNTACTICAL_ERRORS;
                return false;
            }
        } 
        else{ // the non_terminal is in brackets (E) -> E
            if(tmp_first->symbol == RIGHT_B && tmp_third->symbol == LEFT_B){
                if(tmp_second->symbol == NON_TERMINAL){
                    if(tmp_second->current_status == VALID_TOKEN){
                        preserve_token = true;
                    }
                    else if(tmp_second->my_token.type_token == TYPE_IDENTIFIER || tmp_second->my_token.type_token == TYPE_STRING){
                        preserve_token = true;
                    }
                    reduce_brackets(tmp_second, to_pop, stack, preserve_token);
                }
                else{
                    return_code = OTHER_SYNTACTICAL_ERRORS;
                    return false;
                }
            }
            else{
                return_code = OTHER_SYNTACTICAL_ERRORS;
                return false;
            }
        }
    } // other count
    else{
        return_code = OTHER_SYNTACTICAL_ERRORS;
        return false;
    }

    // if it passed all the grammar tests, it was correct
    return true;
}

/**
 * Adds symbol after first terminal symbol
 */
bool add_after_first_terminal(Symbol_stack_t* stack, Precedential_table_symbol symbol){
    bool found = false;
    Symbol_item_t* tmp = stack->top;
    Symbol_item_t* add_after;
    Symbol_item_t* non_terminal;
    Symbol_item_t* new_item = malloc(sizeof(Symbol_item_t));
    if(new_item == NULL){
        return false;
    }

    new_item->symbol = symbol;
    new_item->current_status = INVALID_TOKEN;
    new_item->my_token.type_token = TYPE_EOL;
    new_item->in_def = 0;    
    
    if(is_term(stack->top->symbol)){ // terminal is on top
        new_item->next = stack->top;
        stack->top = new_item;
        return true;
    }
    else{                           // not on top, search through the stack
        while(!found && tmp != NULL){
            if(is_term(tmp->next->symbol)){
                add_after = tmp;
                non_terminal = tmp->next;
                add_after->next = new_item;
                new_item->next = non_terminal;
                return true;
            }
            tmp = tmp->next;
        }    
    }
    return false;
}

/**
 * Evaluates, whether symbol is non-terminal or not
 */
bool is_term(Precedential_table_symbol symbol){
    switch(symbol){
        case NON_TERMINAL:
        case START:
            return false;
        default:
            return true;
    }
}

/**
 * Gets first terminal from the table
 */
Precedential_table_symbol get_first_term(Symbol_stack_t* stack){
    bool found = false;
    Symbol_item_t* tmp = stack->top;

    while(!found){
        if(is_term(tmp->symbol)){
            break;
        }
        tmp = tmp->next;
    }
    return tmp->symbol;
}

/**
 * Computes the rows and columns value and gets the rule
 */
Precedential_table_rule get_indexes_and_rule(Symbol_stack_t* stack, Precedential_table_symbol symbol){
    int rows = get_index(get_first_term(stack));
    int columns = get_index(symbol);
    return get_rule(rows, columns);
}


/**
 * Calculates how many symbols we have to pop
 */
int count_to_reduce(Symbol_stack_t* stack){
    int count = 0;
    Symbol_item_t* tmp = stack->top;

    while(tmp->symbol != START){
        count++;
        tmp = tmp->next;
    }
    return count;
}

/**
 * Pops n symbols from the stack
 */
void pop_count(int n){
    for(int i = 0; i < n; i++){
        pop_stack(&stack);
    }
}

/**
 * Gets rule according to columns and rows
 */
Precedential_table_rule get_rule(Precedential_table_symbol rows, Precedential_table_symbol columns){
    if(rows == (table_size-1) && columns == (table_size-1))
        finished = true;
    return precedential_table[rows][columns];
}

/**
 * Returns error_type when comething goes wrong
 */
int expression_error(Symbol_stack_t* stack, Symbol_list* list,  int error_type){
    free_stack(stack);
    clear_buffer(list);
    return error_type;
}

/**
 * Searches the table
 */
void check_sematics(Data_t* data){
    if(data->in_definition == true){    // local
        if(check_define(local_ST, data->token->attr.string->s) == PARAM_DEFINED){
            return_code = EXPRESSION_OK;
            return;
        }
    }
    else{                               // global
        if(check_define(global_ST, data->token->attr.string->s) == PARAM_DEFINED){
            return_code = EXPRESSION_OK;
            return;
        }
    }                                   // function
    if(check_define(global_ST, data->token->attr.string->s) == FUNCTION_DEFINED){
        return_code = EXPRESSION_OK;
        return;
    }
    return_code = UNDEFINED_ID_EXPRESSION;
    
}

/**
 * Returns symbol of precedential table from token 
 */
Precedential_table_symbol get_symbol_from_token(Data_t* data){
    if(data->token->token == TYPE_KEYWORD){
        if(data->token->attr.keyword == KEYWORD_NIL){
            return ID;
        }
    }
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
            return ID;
        case(TYPE_IDENTIFIER):
            check_sematics(data);
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
        case(TYPE_ASSIGN):
            return_code = OTHER_SYNTACTICAL_ERRORS;
            return DOLLAR;
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
        case(EQL):  // relational operations have the same index
        case(NEQ):
        case(LEQ):
        case(LTN):
        case(MEQ):
        case(MTN):
            return INDEX_RELATIONAL_OPERATION;
        default:
            return INDEX_DOLLAR;
    }
}

/***********************************************************
 * 
 *                  STACK FUNCTIONS
 * 
 **********************************************************/

/**
 * Initializes top of stack to null
 */
void init_stack(Symbol_stack_t* stack){
    stack->top = NULL;
}

/**
 * Pushes symbol to stack
 */
bool push_stack(Symbol_stack_t* stack, Precedential_table_symbol symbol, Data_t* data){
    Symbol_item_t* tmp = malloc(sizeof(Symbol_item_t));
    Symbol_item_t* stack_top;

    if(tmp == NULL){
        return false;
    }
    else{
        tmp->next = stack->top;
        tmp->symbol = symbol;
        stack->top = tmp;

        if(from_lexer){
            remember_token(tmp, data);
            
        }
        else if(from_buffer){
            stack_top = get_from_buffer(&buffer);
            if(stack_top->current_status == VALID_TOKEN){
            tmp->my_token.type_token = stack_top->my_token.type_token;                          
            tmp->my_token.attr_token.tmp_flt = stack_top->my_token.attr_token.tmp_flt;               
            tmp->my_token.attr_token.tmp_integer = stack_top->my_token.attr_token.tmp_integer;       
            tmp->my_token.attr_token.tmp_keyword = stack_top->my_token.attr_token.tmp_keyword;
            tmp->in_def = stack_top->in_def;       
            if(tmp->my_token.type_token == TYPE_STRING || stack_top->my_token.type_token == TYPE_IDENTIFIER){
                int length = strlen(stack_top->my_token.attr_token.tmp_string) + 2;
                tmp->my_token.attr_token.tmp_string = (char *) malloc(length);
                if(tmp->my_token.attr_token.tmp_string == NULL){
                    return ER_INTERNAL;
                } 
                strcpy(tmp->my_token.attr_token.tmp_string, stack_top->my_token.attr_token.tmp_string);
                }
            }
            else if(stack_top->current_status == INVALID_TOKEN){
                tmp->current_status = stack_top->current_status;
                tmp->symbol = stack_top->symbol;
            }
            delete_first(&buffer);

        }
        
        if(symbol != DOLLAR){
            tmp->current_status = VALID_TOKEN;
        }
        else{
            tmp->current_status = INVALID_TOKEN;
        }
        return true;
    }  
}

/**
 * Pushes symbol to stack without tha parameter data, it is INVALID_TOKEN by default
 */
bool push_no_token(Symbol_stack_t* stack, Precedential_table_symbol symbol){
    Symbol_item_t* tmp = malloc(sizeof(Symbol_item_t));

    if(tmp == NULL){
        return false;
    }
    else{
        tmp->next = stack->top;
        tmp->symbol = symbol;
        tmp->current_status = INVALID_TOKEN;
        tmp->my_token.type_token = TYPE_EOL;
        tmp->in_def = 0;
        stack->top = tmp;
        return true;
    }
}

/**
 * Pops symbol from stack
 */
bool pop_stack(Symbol_stack_t* stack){
    Symbol_item_t* tmp = stack->top;
    
    if(tmp->my_token.type_token == TYPE_STRING || tmp->my_token.type_token == TYPE_IDENTIFIER){
        free(tmp->my_token.attr_token.tmp_string);
    }
    
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
        if(tmp->my_token.type_token == TYPE_STRING || tmp->my_token.type_token == TYPE_IDENTIFIER){
            free(tmp->my_token.attr_token.tmp_string);
        }
        stack->top = tmp->next;
        free(tmp);
    }
}

/**
 * Does a deep copy of token and all its attributes
 */
int remember_token(Symbol_item_t* tmp, Data_t* data){
    int length;
    tmp->my_token.type_token = data->token->token;                          
        tmp->my_token.attr_token.tmp_flt = data->token->attr.flt;               
        tmp->my_token.attr_token.tmp_integer = data->token->attr.integer;       
        tmp->my_token.attr_token.tmp_keyword = data->token->attr.keyword;
        tmp->in_def = data->in_definition;       
        if(tmp->my_token.type_token == TYPE_STRING || tmp->my_token.type_token == TYPE_IDENTIFIER){ 
            length = data->token->attr.string->current_size + 1;                    
            tmp->my_token.attr_token.tmp_string = (char *) malloc(length); 
            if(tmp->my_token.attr_token.tmp_string == NULL){    
                return ER_INTERNAL; 
            }
            strcpy(tmp->my_token.attr_token.tmp_string, data->token->attr.string->s);
        }
    return 0;  
}

/***********************************************************
 * 
 *                  BUFFER FUNCTIONS
 * 
 **********************************************************/

/**
 * Initializes buffer at the beginning
 */
void init_buffer(Symbol_list* list){
    list->first = NULL;
    list->last = NULL;
}

/**
 * Clears buffer if error or after expression is handled
 */
void clear_buffer(Symbol_list* list){
    Symbol_item_t* tmp = list->first;
    Symbol_item_t* to_delete;
    return_code = EXPRESSION_OK;
    
    if(tmp == NULL){
        return;
    }
    while(tmp != NULL){
        to_delete = tmp;
        if(tmp == list->last){
            list->last = NULL;
        }

        if(tmp->my_token.type_token == TYPE_STRING || tmp->my_token.type_token == TYPE_IDENTIFIER){
            free(to_delete->my_token.attr_token.tmp_string);
        }
        
        tmp = tmp->next;
        free(to_delete);
    }
    if(list->last == NULL){
        list->first = NULL;
    }
}

/**
 * Inserts item at the end of the buffer
 */
int insert_to_buffer(Symbol_list* list, Data_t* data){
    Precedential_table_symbol current_symbol = get_symbol_from_token(data);
    Symbol_item_t* last_one;

    Symbol_item_t* tmp = malloc(sizeof(Symbol_item_t));
    if(tmp == NULL){
        return ER_INTERNAL;
    }

    tmp->symbol = current_symbol;
    tmp->next = NULL;
    tmp->my_token.type_token = TYPE_EOL;

    if(current_symbol != DOLLAR){
        remember_token(tmp, data);
        tmp->current_status = VALID_TOKEN;
    }
    else{
        tmp->current_status = INVALID_TOKEN;
    }
    
    if(list->first == NULL && list->last == NULL){ // zero elements
        list->first = tmp;
        list->last = tmp;
    }
    else if(list->first != NULL && list->first == list->last){ // one element
        list->last = tmp;
        list->first->next = tmp;
    }
    else{   // more elements
        last_one = list->last;
        last_one->next = tmp;
        list->last = tmp;
    }
    
    return 0;
}

/**
 * Inserts stop to buffer so we know when to stop handling expression
 */
int insert_stop(Symbol_list* list){
    Precedential_table_symbol stop_sign = DOLLAR;
    Symbol_item_t* last_one;

    Symbol_item_t* tmp = malloc(sizeof(Symbol_item_t));
    if(tmp == NULL){
        return ER_INTERNAL;
    }

    tmp->symbol = stop_sign;
    tmp->current_status = VALID_TOKEN;
    tmp->my_token.type_token = TYPE_EOL;
    tmp->in_def = 0;
    tmp->next = NULL;

    if(list->first == NULL && list->last == NULL){ // zero elements
        list->first = tmp;
        list->last = tmp;
    }
    else if(list->first != NULL && list->first == list->last){ // one element
        list->last = tmp;
        list->first->next = tmp;
    }
    else{   // more elements
        last_one = list->last;
        last_one->next = tmp;
        list->last = tmp;
    }
    return 0;
}

/**
 * Returns first item from the list
 */
Symbol_item_t* get_from_buffer(Symbol_list* list){
    return (list->first);
}

/**
 * Deletes first item from the list and binds it together
 */
void delete_first(Symbol_list* list){
    if(is_empty(list))
        return;

    Symbol_item_t* tmp = list->first;
    if(list->first == list->last){  // there is only one element
        list->first = NULL;
        list->last = NULL;       
    }
    else{                           // more elements
        list->first = list->first->next;
    }
    if(tmp->my_token.type_token == TYPE_STRING || tmp->my_token.type_token == TYPE_IDENTIFIER){
        free(tmp->my_token.attr_token.tmp_string);
    }
    free(tmp);
}

/**
 * Checks whether list is empty or not
 */
bool is_empty(Symbol_list* list){
    if(list->first == NULL && list->last == NULL)
        return true;
    else
        return false;
}

