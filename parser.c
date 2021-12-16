/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__1
#else
#define _POSIX_C_SOURCE 200809L
#endif



#include "error.h"
#include "parser.h"
#include "expression.h"
#include "instructions.h"


#define GET_TOKEN()                                                                             \
    do { res = get_next_token(data->token);                                                     \
        if (res != LEXER_OK)                                                                    \
            return(res);                                                                        \
    } while (data->token->token == TYPE_COMMENT);                                               \
   

#define IS_VALUE()                                                                              \
    data->token->token == TYPE_INT || data->token->token == TYPE_FLOAT                          \
    || data->token->token == TYPE_STRING || data->token->token == TYPE_IDENTIFIER               \
    || (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_NIL)

#define IF_N_OK_RETURN(__func__)                                                                \
    res = __func__;                                                                             \
    if (res != SYN_OK) return(res);    

#define IS_OPERAND()                                                                                        \
    data->token->token == TYPE_PLUS ||data->token->token == TYPE_MINUS || data->token->token == TYPE_MUL    \
    || data->token->token == TYPE_DIV || data->token->token == TYPE_NEQ || data->token->token == TYPE_LEQ   \
    || data->token->token == TYPE_LTN || data->token->token == TYPE_MEQ || data->token->token == TYPE_MTN   \
    || data->token->token == TYPE_EQ        

#define CHECK_BOTH_TABLES()                                                                     \
    if (data->in_definition == true) {                                                          \
        if (check_define(local_ST, data->token->attr.string->s) != PARAM_DEFINED) {             \
            return (ER_SEM_VARIABLE);                                                           \
        }                                                                                       \
    } else                                                                                      \
        if (check_define(global_ST, data->token->attr.string->s) != PARAM_DEFINED) {            \
            return(ER_SEM_VARIABLE);                                                            \
    } 

#define CHECK_GEN(gen_func)                                                                     \
    if (gen_func == false)                                                                      \
        return(99);


// temporary variables to keep ID from token for later saving into SymTable
string_t identifier_f;
string_t identifier;
string_t identifier_declare;

int res;                // tmp result for checking returned values
int params_cnt = 0;     // counter of parameters in functions
int if_whlie_idx = 0;   // index of IF or WHILE for generating labels in code

tHTItem tItem;          // item of SymTable needed for actualizing SymTable




void save_id(string_t* identifier, Data_t* data) {
    add_string(identifier, data->token->attr.string->s);
}



/* ***************************************************************************************
 * <prog>
 * ***************************************************************************************/
int prog(Data_t* data){

    GET_TOKEN();

    // <prog> -> DEF ID_FUNC ( <params> ) EOL <statement> END <prog>
    if(data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_DEF){
        if (data->in_definition == false) {
            data->in_definition = true;
        } else {
            return(ER_SYN);
        }

        // ... ID_FUNC ...
        GET_TOKEN();

        if (data->token->token != TYPE_IDENTIFIER && data->token->token != TYPE_FUNC) {
            return(ER_SYN);
        }

        save_id(&identifier_f, data);
        
        CHECK_GEN(gen_func_start((&identifier_f)->s))

        // ... ( ...
        GET_TOKEN();
        if (data->token->token != TYPE_LEFT_BRACKET) 
            return(ER_SYN);
        
        // ... <params> ...
        GET_TOKEN();
        params_cnt = 0;
        IF_N_OK_RETURN(params(data));
        
        // ... ) ... - don't call GET_TOKEN - function will return with new token
        if (data->token->token != TYPE_RIGHT_BRACKET) 
            return(ER_SYN);
    
        // ... EOL ...
        GET_TOKEN();
        if (data->token->token != TYPE_EOL) 
            return(ER_SYN);

        // uptade ID_FUNC in SymTable
        itemupdate(&tItem, (&identifier_f)->s,  FUNCTION, true, params_cnt);
        res = htInsert(global_ST, &tItem);
        if (res != ST_OK) {
            return(res);
        }
        params_cnt = 0;

        
        // ... <statement> ...
        GET_TOKEN();
        IF_N_OK_RETURN(statement(data));
        

        // ... END ...  - - don't call GET_TOKEN - function will return with new token
        // function will return here only if token == END
        if (!(data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_END))
            return(ER_SYN);


        // ... EOL || EOF ... 
        GET_TOKEN();
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF) {
            htClearlocal();
            return(prog(data));
        }
            
    } 


    // <prog> -> EOL <prog>
    else if(data->token->token == TYPE_EOL){
        return(prog(data));
    } 


    // <prog> -> EOF
    else if(data->token->token == TYPE_EOF){
        return SYN_OK;
    } 


    // <prog> -> <statements> <prog>
    else if(data->token->token == TYPE_KEYWORD || data->token->token == TYPE_IDENTIFIER || data->token->token == TYPE_FUNC){
        
        return(statement(data));  
    }

    // ... INT/FLT/STR ...
    else if (IS_VALUE() || data->token->token == TYPE_LEFT_BRACKET) {

        IF_N_OK_RETURN(insert_to_buffer(&buffer, data));

        while (data->token->token == TYPE_LEFT_BRACKET) {
            GET_TOKEN();
            IF_N_OK_RETURN(insert_to_buffer(&buffer, data));
        }

        // need to check what's next token before calling <expression>
        GET_TOKEN();

        IF_N_OK_RETURN(insert_to_buffer(&buffer, data));

        // checking if operand follows after value
        // for example 0variable=5 -> SyntaxErr
        if (data->token->token != TYPE_EOL && !(IS_OPERAND()) && data->token->token != TYPE_RIGHT_BRACKET) {
            return(ER_SYN);
        }

        res = handle_expression(data);
        if (res != EXPRESSION_OK) {
            return(res);
        }

        clear_buffer(&buffer);
        if (res != EXPRESSION_OK) {
            return(res);
        }

        return(prog(data));
    }
    
        
    return ER_SYN;
}



/* ***************************************************************************************
 * <statement>
 * ***************************************************************************************/
int statement(Data_t* data) {

    // <statement> -> IF <expression> THEN EOL <statements> ELSE EOL <statements> END EOL
    // ... IF ...
    if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_IF) {
        data->in_while_or_if++;
        if_whlie_idx +=2;
        int this_if = if_whlie_idx;

        // ... <expression> ...
        
        GET_TOKEN();

        if (!(IS_VALUE()) && (data->token->token != TYPE_LEFT_BRACKET)) {
            return(ER_SYN);
        }
        IF_N_OK_RETURN(insert_to_buffer(&buffer, data));

        res = handle_expression(data);
        if (res == EXPRESSION_OK) {                                   
        } else 
            return(res);                     

        clear_buffer(&buffer); 
        
        // ... THEN ...
        if (data->token->token != TYPE_KEYWORD || data->token->attr.keyword != KEYWORD_THEN) {
            return(ER_SYN);
        }

        if (data->in_while_or_if == 1) 
             CHECK_GEN(gen_new_part())
    
         CHECK_GEN(gen_if_start(this_if, data->in_while_or_if))
        // ... EOL ...
        GET_TOKEN();
        if (data->token->token != TYPE_EOL) {
            return(ER_SYN);
        }

        // ... <statements> ...
        GET_TOKEN();
        // check first part of IF
        IF_N_OK_RETURN(statement(data));
    
        // ... ELSE ...
        // ELSE
        if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_ELSE) {
            // ... EOL ...
            GET_TOKEN();
            if (data->token->token != TYPE_EOL) {
                return(ER_SYN);
            }
          
            CHECK_GEN(gen_if_else(this_if, data->in_while_or_if))
            // ... <statements> ...
            GET_TOKEN();
            // check second part of IF
            IF_N_OK_RETURN(statement(data));
        } else {
            return(ER_SYN);
        }

        

        
        
        // ... END ...  - don't call GET_TOKEN - function will return with new token
        if (!(data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_END))
            return(ER_SYN);

         CHECK_GEN(gen_if_end(this_if+1, data->in_while_or_if+1))

        // ... EOL || EOF ... 
        GET_TOKEN();
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF)   
            return(prog(data));
        

        // if nothing good after IF, return ERROR  
        return(ER_SYN);
    }
    

    // <statement> -> WHILE <expression> DO EOL <statement> END EOL
    // ... WHILE ...
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_WHILE) {
        data->in_while_or_if++;
        if_whlie_idx +=2;
        int this_while = if_whlie_idx;

        if (data->in_while_or_if == 1) 
             CHECK_GEN(gen_new_part())

         CHECK_GEN(gen_while_header(this_while, data->in_while_or_if))


        GET_TOKEN();

        if (!(IS_VALUE()) && (data->token->token != TYPE_LEFT_BRACKET)) {
            return(ER_SYN);
        }
        IF_N_OK_RETURN(insert_to_buffer(&buffer, data));

        // ... <expression> ...
        res = handle_expression(data);
        if (res != EXPRESSION_OK) {      
            return(res);
        }

        clear_buffer(&buffer);
        
        // ... DO ...
        if (data->token->token != TYPE_KEYWORD || data->token->attr.keyword != KEYWORD_DO)
            return(ER_SYN);
    
        // ... EOL ...
        GET_TOKEN();
        if (data->token->token != TYPE_EOL)
            return(ER_SYN);

       
        CHECK_GEN(gen_while_start(this_while+1, data->in_while_or_if))
        
        // ... <statements> ...
        GET_TOKEN();
        IF_N_OK_RETURN(statement(data));
        
        // ... END ...  - don't call GET_TOKEN - function will return with new token
        if (!(data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_END))
            return(ER_SYN);

        gen_while_end(this_while+1, data->in_while_or_if+1);

        // ... EOL || EOF ... 
        GET_TOKEN();
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF)   
            return(prog(data));
    

        // if nothing good after WHILE, return ERROR
        return(ER_SYN);
    }

    // <statement> -> <function> EOL
    // for built in functions
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword > 8 && data->token->attr.keyword < 17) {
        // ... <function> ...
        IF_N_OK_RETURN(function(data));

        // ... EOL || EOF ... - don't call GET_TOKEN - function will return with new token
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF)   
            return(prog(data));
    }
    

    // <statement> -> ID <declare> EOL
    // <statement> -> ID_FUNC ( <argvs> ) EOL
    // if ID ends with "?" or "!", can't be created variable
    else if (data->token->token == TYPE_FUNC) {
        save_id(&identifier, data);
        save_id(&identifier_declare, data);
        

        GET_TOKEN();

        if (data->token->token == TYPE_ASSIGN) {
            return(ER_SEM_OTHER);
        }

        // <statement> -> ID_FUNC 
        // function is defined
        if (check_define(global_ST, (&identifier)->s) == FUNCTION_DEFINED) {
            
            CHECK_GEN(gen_func_prep_for_params())

            IF_N_OK_RETURN(argvs(data));

            CHECK_GEN(gen_func_call((&identifier)->s))

            return(prog(data));
        } else 

        // we are in function and called function isn't defined
        if (data->in_definition == true) {
            IF_N_OK_RETURN(argvs(data));

            itemupdate(&tItem, (&identifier)->s, FUNCTION, false, params_cnt);
            res = htInsert(global_ST, &tItem);
            if (res != ST_OK) {
                return(res);
            }

        // function isn't defined and we are not in definition
        } else {
            return(ER_SEM_VARIABLE);
        }
            
    }

    else if (data->token->token == TYPE_IDENTIFIER || data->token->token == TYPE_FUNC) {


        save_id(&identifier, data);
        save_id(&identifier_declare, data);

        IF_N_OK_RETURN(insert_to_buffer(&buffer, data));

        // func?/func! ...
        if (data->token->token == TYPE_FUNC) {
            GET_TOKEN();

            if (data->token->token == TYPE_ASSIGN) {
                return(ER_SEM_OTHER);
            }
        }

        // ID ___... 
        if (data->token->token == TYPE_IDENTIFIER) {   
            GET_TOKEN();
        }

        IF_N_OK_RETURN(insert_to_buffer(&buffer, data));

        // <statement> -> ID_FUNC 
        if (check_define(global_ST, (&identifier)->s) == FUNCTION_DEFINED) {
            clear_buffer(&buffer);

            if (data->token->token == TYPE_ASSIGN) {
                return(ER_SEM_VARIABLE);
            }

            CHECK_GEN(gen_func_prep_for_params())

            IF_N_OK_RETURN(argvs(data));

            CHECK_GEN(gen_func_call((&identifier)->s))

            return(prog(data));

        }


        // <statement> -> ID EOL || EOF 
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF) {
            clear_buffer(&buffer);
            itemupdate(&tItem, (&identifier)->s,  VAR, false, 0);

            // we are in Definition
            if (data->in_definition == true) {
                if (check_define(local_ST, identifier_declare.s) != PARAM_DEFINED) {
                    return(ER_SEM_VARIABLE);
                }
            
            // we aren't in Definition
            } else {
                if (check_define(global_ST, identifier_declare.s) != PARAM_DEFINED) {
                    return(ER_SEM_VARIABLE);
                }
            }
            
            return(prog(data));
        } else



        // <statement> -> ID <declare> EOL
        // ... = ...
        if (data->token->token == TYPE_ASSIGN) {
            clear_buffer(&buffer);

            // when in Definition, searching in local table
            if (data->in_definition == true) {
                if (check_define(local_ST, identifier_declare.s) != PARAM_DEFINED) {
                    if(data->in_while_or_if == 0)
                    {
                        CHECK_GEN(gen_var_declar(identifier_declare.s))
                    }
                    else
                    {
                        CHECK_GEN(gen_defvar_2_old(identifier_declare.s))
                    }
                }
            // when not, searching in global table
            } else {
                if (check_define(global_ST, identifier_declare.s) != PARAM_DEFINED) {
                    if(data->in_while_or_if == 0)
                    {
                        CHECK_GEN(gen_var_declar(identifier_declare.s))
                    }
                    else
                    {
                        CHECK_GEN(gen_defvar_2_old(identifier_declare.s))
                    }
                }
            }
            
            itemupdate(&tItem, (&identifier_declare)->s, VAR, true, 0);

            // update ID in SymTable
            if (data->in_definition == true) {
                res = htInsert(local_ST, &tItem);
                if (res != ST_OK) {
                    return(res);
                }
                //htPrintTable(local_ST);

            } else {
                res = htInsert(global_ST, &tItem);
                if (res != ST_OK) {
                    return(res);
                }
                //htPrintTable(global_ST);
            }

            res = declare(data);
            if (res!= SYN_OK) {
                return(res);
            }
            return(prog(data));
        } else
    
        

        // CALLING FUNCTION BEFORE ITS DEFINITION
        // <statement> -> ID_FUNC ( <argvs> )
        if (data->token->token == TYPE_LEFT_BRACKET || IS_VALUE()) {
            clear_buffer(&buffer);

            if (check_define(global_ST, (&identifier)->s) != FUNCTION_DEFINED && check_define(global_ST, (&identifier)->s) != PARAM_DEFINED && data->in_definition == true) {
                IF_N_OK_RETURN(argvs(data));

                itemupdate(&tItem, (&identifier)->s, FUNCTION, false, params_cnt);
                res = htInsert(global_ST, &tItem);
                if (res != ST_OK) {
                    return(res);
                }
                //htPrintTable(global_ST);
            } else {
                return(ER_SEM_VARIABLE);
            }

            return(prog(data));

        } else 
        
        // expression at begin of line
        if (IS_OPERAND() || data->token->token) {
            res = handle_expression(data);
            clear_buffer(&buffer);
            if (res != EXPRESSION_OK) {
                return(res);
            } else {
                return(prog(data));
            }
            
        } else {

            return(ER_SYN);
        }
    } else

    // expression at the begin of line  ... VALUE || ( ...
    if (IS_VALUE() || data->token->token == TYPE_LEFT_BRACKET) {
        IF_N_OK_RETURN(insert_to_buffer(&buffer, data));
        
        // getting tokens until get something != "("
        while (data->token->token == TYPE_LEFT_BRACKET) {
            GET_TOKEN();
            IF_N_OK_RETURN(insert_to_buffer(&buffer, data));

            if (!(IS_VALUE())) {
                return(ER_SYN);
            }
        }
        GET_TOKEN();

        IF_N_OK_RETURN(insert_to_buffer(&buffer, data));

        // others expressions 
        if (IS_OPERAND() || data->token->token == TYPE_EOL || data->token->token == TYPE_EOF) {
            res = handle_expression(data);
            clear_buffer(&buffer);
            if (res != EXPRESSION_OK) {
                return(res);
            } else {
                return(prog(data));
            }
            
        } else {
            return(ER_SYN);
        }

    }

    

    // <statement> -> EOL <prog>
    // allows tak emply lines as statement
    if (data->token->token == TYPE_EOL)
        return(prog(data));

    // ****************************************
    // make <statement> return when END or ELSE
    if (data->token->token == TYPE_KEYWORD && (data->token->attr.keyword == KEYWORD_END || data->token->attr.keyword == KEYWORD_ELSE)) {
        // end of IF/WHILE
        if (data->in_while_or_if != 0) {
            if (data->token->attr.keyword == KEYWORD_END) {

                data->in_while_or_if--;
                return(SYN_OK);
            }
            if (data->token->attr.keyword == KEYWORD_ELSE) {
                return(SYN_OK);
            }
        } else 
        // end of DEFINITION
        if (data->in_definition == true) {
            if (data->token->attr.keyword == KEYWORD_END) {
                data->in_definition = false;

                CHECK_GEN(gen_func_rval())
                CHECK_GEN(gen_func_ret((&identifier_f)->s))
                CHECK_GEN(gen_func_end((&identifier_f)->s))

                return(SYN_OK);
            }
        } else
            return(ER_SYN);
    }


    return(ER_SYN);
}



/* ***************************************************************************************
 * <declare>
 * ***************************************************************************************/
int declare(Data_t* data) {
    // <declare> =
    data->in_declare = true;

    GET_TOKEN();

    // ID, ID_FUNC, int/flt/str
    if (IS_VALUE() || data->token->token == TYPE_LEFT_BRACKET || data->token->token == TYPE_FUNC) {
        IF_N_OK_RETURN(insert_to_buffer(&buffer, data));

        // ... ID, ID_FUNC ...
        if (data->token->token == TYPE_IDENTIFIER || data->token->token == TYPE_FUNC) {

            save_id(&identifier, data);
            
            // ... ID_FUNC ...
            if (check_define(global_ST, data->token->attr.string->s) == FUNCTION_DEFINED) {
                
                CHECK_GEN(gen_func_prep_for_params())

                GET_TOKEN();

                IF_N_OK_RETURN(argvs(data));
                //itemupdate(&tItem, (&identifier_declare)->s, VAR, true, 0);
                
                
                
                CHECK_GEN(gen_func_call((&identifier)->s))
                CHECK_GEN(gen_func_rval_assign((&identifier_declare)->s))

                clear_buffer(&buffer);
                data->in_declare = false;
                return(SYN_OK);

            } else

            // ... ID ... 
            if ((data->in_definition == false && check_define(global_ST, identifier.s) == PARAM_DEFINED) ||
                (data->in_definition == true && check_define(local_ST, identifier.s) == PARAM_DEFINED)) {
                
                // handle expression and assign value to variable
                res = handle_expression(data);
                if (res != EXPRESSION_OK) {
                    clear_buffer(&buffer);
                    return(res);
                }

                CHECK_GEN(gen_assign(identifier_declare.s))

                clear_buffer(&buffer);
                data->in_declare = false;
                return(SYN_OK);

            } else

            // FUNC - if not defined            
            if (data->in_definition == true) {
                if (data->token->token == TYPE_FUNC) {
                    IF_N_OK_RETURN(argvs(data));

                    itemupdate(&tItem, (&identifier)->s, FUNCTION, false, params_cnt);
                    res = htInsert(global_ST, &tItem);
                    if (res != ST_OK) {
                        return(res);
                    }

                } else {
                    GET_TOKEN();
                    if (data->token->token == TYPE_LEFT_BRACKET || IS_VALUE()) {
                       IF_N_OK_RETURN(argvs(data));

                        itemupdate(&tItem, (&identifier)->s, FUNCTION, false, params_cnt);
                        res = htInsert(global_ST, &tItem);
                        if (res != ST_OK) {
                            return(res);
                        } 
                    } else {
                        return(ER_SYN);
                    } 
                }
            }

            // if nothing good -> ERR
            else {
                clear_buffer(&buffer);
                return(ER_SEM_VARIABLE);
            }
        } 
        // ... int/flt/str ...
        else {

            res = handle_expression(data);
            if (res != EXPRESSION_OK) {
                clear_buffer(&buffer);
                return(res);
            }

            CHECK_GEN(gen_assign(identifier_declare.s))

            clear_buffer(&buffer);
            data->in_declare = false;
            return(SYN_OK);

        
        }

        clear_buffer(&buffer);

        data->in_declare = false;
        return(SYN_OK);
    } else 

    // built in functions
    if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword > 8 && data->token->attr.keyword < 17) {
        
        res = function(data);
        if (res != SYN_OK) {
            return(res);
        } else {
            data->in_declare = false;
            return(SYN_OK);
        }
    } else 

    // expressions starting with "("
    if (data->token->token == TYPE_LEFT_BRACKET) {
        res = handle_expression(data);
        if (res != EXPRESSION_OK) {
            return(res);
        }
        data->in_declare = false;
        return(SYN_OK);
    }


    // <declare> -> ε
    return(ER_SYN);
}


/* ***************************************************************************************
 * <params>
 * ***************************************************************************************/
int params(Data_t* data) {
    // <params> -> ID <param>
    // ... ID ...

    if (data->token->token == TYPE_IDENTIFIER) {
        int res;
        res = param(data);

        return(res);
        //return(param(data));
    } else

    // <params> -> ε
    // ... ) ... - don't call GET_TOKEN - only if "func ()" - without parameters
    if (data->token->token == TYPE_RIGHT_BRACKET) {
        return(SYN_OK);
    }

    return(ER_SYN);
}

int param(Data_t* data) {
    // <param> , ID <param>
    // ... ID ... - needs to check ID again for case of recursive calling
    if (data->token->token == TYPE_IDENTIFIER) {
    
        params_cnt++;
        save_id(&identifier, data);

        CHECK_GEN(gen_func_param((&identifier)->s, params_cnt))

        res = def_ID(local_ST, (&identifier)->s);
        if (res != ST_OK) {
            return(res);
        }

        // check if next parameter
        GET_TOKEN();
        // ... , ...
        if (data->token->token == TYPE_COMMA) {
            // now check next parameter for sure
            GET_TOKEN();
            return(param(data));
        
        } else
        // ... ) ...
        if (data->token->token == TYPE_RIGHT_BRACKET) {
            return(params(data));
        }
    }

    // <param> -> ε
    return(ER_SYN);
}


/* ***************************************************************************************
 * <argvs>
 * ***************************************************************************************/
int argvs(Data_t* data) {
    // <argvs> -> <value> <arg>
    // ... <value> ...

    params_cnt = 0;

    // ... ( ... - optional
    if (data->token->token == TYPE_LEFT_BRACKET) {
        data->in_bracket = true;
        GET_TOKEN();
    }

    IF_N_OK_RETURN(arg(data));

    if (check_define(global_ST, (&identifier)->s) == FUNCTION_DEFINED) {
        if (params_cnt != check_param_cnt((&identifier)->s)) {
            return(ER_SEM_PARAMETERS);
        }
    }
    
    
    return(SYN_OK);
    // <argvs> -> ε
}


/* ***************************************************************************************
 * <arg>
 * ***************************************************************************************/
int arg(Data_t* data) {
    // <arg> -> , <value> <arg>
    // <arg> -> ε    

    if (IS_VALUE()) {
        params_cnt++;
    
        if (data->token->token == TYPE_IDENTIFIER) {

            if (data->in_definition == true) {
                if (check_define(local_ST, data->token->attr.string->s) != PARAM_DEFINED) {
                    return(ER_SEM_VARIABLE);
                }
            } else {
                if (check_define(global_ST, data->token->attr.string->s) != PARAM_DEFINED) {
                    return(ER_SEM_VARIABLE);
                }
            }
        }

        CHECK_GEN(gen_func_pass_param(*(data->token), params_cnt))

        GET_TOKEN();
        // ... , ...
        if (data->token->token == TYPE_COMMA) {

            GET_TOKEN();
            return(arg(data));
        
        } else {
            // ... ) ... - just if was used "("
            if (data->in_bracket == true) {
                if (data->token->token == TYPE_RIGHT_BRACKET) {
                    data->in_bracket = false;
                    GET_TOKEN();
                } else {
                    return(ER_SYN);
                }
            }
            
            // ... EOL || EOF ...
            if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF) {
                return(SYN_OK);
            }
        }

        return(ER_SYN);
    }

    
        // ... ) ... - just if was used "("
        if (data->in_bracket == true) {
            if (data->token->token == TYPE_RIGHT_BRACKET) {
                data->in_bracket = false;
                GET_TOKEN();
            } else {
                return(ER_SYN);
            }
        }

        // ... EOL || EOF ...
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF) {
            return(SYN_OK);
        }

    return(ER_SYN);
}




/* ***************************************************************************************
 * <function>
 * ***************************************************************************************/
int function(Data_t* data) {

    // <function> -> PRINT ( <argvs> ) EOL
    if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_PRINT) {

        GET_TOKEN();

        // ... ( ... - optional
        if (data->token->token == TYPE_LEFT_BRACKET) {
            data->in_bracket = true;
            GET_TOKEN();
        }

        clear_buffer(&buffer);
        // non-zero nzmber of args - if next was ")" ot EOL/EOF -> ER_SYN
        if (IS_VALUE()) {
            IF_N_OK_RETURN(print(data));


            // ... EOL || EOF ... - already checked token
            return(SYN_OK);
        } else
        return(ER_SYN);
    }


    // <function> -> LENGTH ( <argvs> ) EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_LENGTH) {

        CHECK_GEN(gen_func_prep_for_params())
        
        GET_TOKEN();

        // ... ( ... - optional
        if (data->token->token == TYPE_LEFT_BRACKET) {
            data->in_bracket = true;
            GET_TOKEN();
        }

        // ... string ...
        if (data->token->token == TYPE_STRING) {
            ;
        } else 

        // ... ID ...
        if (data->token->token == TYPE_IDENTIFIER) {
            // check ID in table 
            CHECK_BOTH_TABLES();
        
        } 
        
        
        // everything else - ERR
        else {
            return(ER_SEM_TYPE);
        }
        CHECK_GEN(gen_func_pass_param(*(data->token), 0))

        GET_TOKEN();
        
        // ... ) ... - just if "(" was used
        if (data->in_bracket == true) {
            if (data->token->token == TYPE_RIGHT_BRACKET) {
                data->in_bracket = false;
                GET_TOKEN();
            } else {
                return(ER_SYN);
            }
        }

        // ... EOL || EOF ...
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF) {
            CHECK_GEN(gen_func_call("length"))
            if (data->in_declare == true) {
                CHECK_GEN(gen_func_rval_assign(identifier_declare.s))
            }
            return(SYN_OK);
        }

        return(ER_SYN);
    }


    // <function> -> SUBSTR ( s, i, n ) EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_SUBSTR) {

        CHECK_GEN(gen_func_prep_for_params())
        
        GET_TOKEN();

        // ... ( ... - optional
        if (data->token->token == TYPE_LEFT_BRACKET) {
            data->in_bracket = true;
            GET_TOKEN();
        }

        /***
         * FIRST PARAMETER - s
         ***/
        // ... string ...
        if (data->token->token == TYPE_STRING) {
            // pass

        } else 

        // ... ID ...
        if (data->token->token == TYPE_IDENTIFIER) {
            // check ID in table 
            CHECK_BOTH_TABLES();
        
        } 
        
        // everything else - ERR
        else {
            return(ER_SEM_TYPE);
        }
        CHECK_GEN(gen_func_pass_param(*(data->token), 0))

        GET_TOKEN();

        // ... , ...
        if (data->token->token == TYPE_COMMA) {
            GET_TOKEN();
        } else {
            return(ER_SYN);
        }


        /***
         * SECOND PARAMETER - i
         ***/
        // ... int ...
        if (data->token->token == TYPE_INT) {
            // pass

        } else 

        // ... ID ...
        if (data->token->token == TYPE_IDENTIFIER) {
            // check ID in table
            CHECK_BOTH_TABLES();
        
        } 
        
        // everything else - ERR
        else {
            return(ER_SEM_TYPE);
        }
        CHECK_GEN(gen_func_pass_param(*(data->token), 1))
        GET_TOKEN();

        // ... , ...
        if (data->token->token == TYPE_COMMA) {
            GET_TOKEN();
        } else {
            return(ER_SYN);
        }


        /***
         * THIRD PARAMETER - n
         ***/
        // ... int ...
        if (data->token->token == TYPE_INT) {
            // pass

        } else 

        // ... ID ...
        if (data->token->token == TYPE_IDENTIFIER) {
            // check ID in table 
            CHECK_BOTH_TABLES();
        
        } 
        
        // everything else - ERR
        else {
            return(ER_SEM_TYPE);
        }

        CHECK_GEN(gen_func_pass_param(*(data->token), 2))
        GET_TOKEN();


        // ... ) ... - just if "(" was used
        if (data->in_bracket == true) {
            if (data->token->token == TYPE_RIGHT_BRACKET) {
                data->in_bracket = false;
                GET_TOKEN();
            } else {
                return(ER_SYN);
            }
        }

        // ... EOL || EOF ...
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF) {
            CHECK_GEN(gen_func_call("substr"))
            if (data->in_declare == true) {
                CHECK_GEN(gen_func_rval_assign(identifier_declare.s))
            }
            return(SYN_OK);
        }

        return(ER_SYN);
    }


    // <function> -> ORD ( s, i ) EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_ORD) {

        CHECK_GEN(gen_func_prep_for_params())
        
        GET_TOKEN();

        // ... ( ... - optional
        if (data->token->token == TYPE_LEFT_BRACKET) {
            data->in_bracket = true;
            GET_TOKEN();
        }

        /***
         * FIRST PARAMETER - s
         ***/
        // ... string ...
        if (data->token->token == TYPE_STRING) {
            // pass

        } else 

        // ... ID ...
        if (data->token->token == TYPE_IDENTIFIER) {
            // check ID in table 
            CHECK_BOTH_TABLES();
        
        } 
        
        // everything else - ERR
        else {
            return(ER_SEM_TYPE);
        }

        CHECK_GEN(gen_func_pass_param(*(data->token), 0))

        GET_TOKEN();

        // ... , ...
        if (data->token->token == TYPE_COMMA) {
            GET_TOKEN();
        } else {
            return(ER_SYN);
        }


        /***
         * SECOND PARAMETER - i
         ***/
        // ... int ...
        if (data->token->token == TYPE_INT) {
            // pass

        } else 

        // ... ID ...
        if (data->token->token == TYPE_IDENTIFIER) {
            // check ID in table
            CHECK_BOTH_TABLES();

        } 
        
        // everything else - ERR
        else {
            return(ER_SEM_TYPE);
        }

        CHECK_GEN(gen_func_pass_param(*(data->token), 1))

        GET_TOKEN();


        // ... ) ... - just if "(" was used
        if (data->in_bracket == true) {
            if (data->token->token == TYPE_RIGHT_BRACKET) {
                data->in_bracket = false;
                GET_TOKEN();
            } else {
                return(ER_SYN);
            }
        }

        // ... EOL || EOF ...
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF) {
            CHECK_GEN(gen_func_call("ord"))
            if (data->in_declare == true) {
                CHECK_GEN(gen_func_rval_assign(identifier_declare.s))
            }
            return(SYN_OK);
        }

        return(ER_SYN);
    }


    // <function> -> CHR ( i ) EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_CHR) {

        CHECK_GEN(gen_func_prep_for_params())
       
        GET_TOKEN();

        // ... ( ... - optional
        if (data->token->token == TYPE_LEFT_BRACKET) {
            data->in_bracket = true;
            GET_TOKEN();
        }

        // ... int ...
        if (data->token->token == TYPE_INT) {
            // pass

        } else 

        // ... ID ...
        if (data->token->token == TYPE_IDENTIFIER) {
            // check ID in table - musi byt typu int
            CHECK_BOTH_TABLES();
        } 
        
        // everything else - ERR
        else {
            return(ER_SEM_TYPE);
        }

        CHECK_GEN(gen_func_pass_param(*(data->token), 0))

        GET_TOKEN();


        // ... ) ... - just if "(" was used
        if (data->in_bracket == true) {
            if (data->token->token == TYPE_RIGHT_BRACKET) {
                data->in_bracket = false;
                GET_TOKEN();
            } else {
                return(ER_SYN);
            }
        }

        // ... EOL || EOF ...
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF) {
            CHECK_GEN(gen_func_call("chr"))
            if (data->in_declare == true) {
                CHECK_GEN(gen_func_rval_assign(identifier_declare.s))
            }
            return(SYN_OK);
        }

        return(ER_SYN);

    }


    // <function> -> INPUTS EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_INPUTS) {
    
        GET_TOKEN();

        // ... ( ... - optional
        if (data->token->token == TYPE_LEFT_BRACKET) {
            GET_TOKEN();

            // ... ) ...
            if (data->token->token == TYPE_RIGHT_BRACKET) {
                GET_TOKEN();
            } else {
                return(ER_SYN);
            }
        }

        CHECK_GEN(gen_input((&identifier)->s, STRING, data->in_declare))

        // ... EOL || EOF ...
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF) {
            data->in_declare = false;
            return(SYN_OK);
        }

        return(ER_SYN);
    }

    
    // <function> -> INPUTI EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_INPUTI) {
    
        GET_TOKEN();

        // ... ( ... - optional
        if (data->token->token == TYPE_LEFT_BRACKET) {
            GET_TOKEN();

            // ... ) ...
            if (data->token->token == TYPE_RIGHT_BRACKET) {
                GET_TOKEN();
            } else {
                return(ER_SYN);
            }
        }

        CHECK_GEN(gen_input((&identifier)->s, INTEGER, data->in_declare))

        // ... EOL || EOF ...
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF) {
            data->in_declare = false;
            return(SYN_OK);
        }

        return(ER_SYN);
    }


    // <function> -> INPUTF EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_INPUTF) {
    
        GET_TOKEN();

        // ... ( ... - optional
        if (data->token->token == TYPE_LEFT_BRACKET) {
            GET_TOKEN();

            // ... ) ...
            if (data->token->token == TYPE_RIGHT_BRACKET) {
                GET_TOKEN();
            } else {
                return(ER_SYN);
            }
        }

        CHECK_GEN(gen_input((&identifier)->s, PRASATKO_S_PAPUCKAMI_FLT, data->in_declare))
        // ... EOL || EOF ...
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF) {
            data->in_declare = false;
            return(SYN_OK);
        }

        return(ER_SYN);
    }

    return(ER_SYN);
}

/* ***************************************************************************************
 * PRINT 
 * is written as separated function due to optional number of parameters
 * every next parameter calls this function again
 * ***************************************************************************************/
int print(Data_t* data) {

    clear_buffer(&buffer);
    while (data->token->token != TYPE_COMMA && data->token->token != TYPE_EOL && data->token->token != TYPE_EOF) {
        if (data->in_bracket == true && data->token->token == TYPE_RIGHT_BRACKET) {
            break;
        }
        
        IF_N_OK_RETURN(insert_to_buffer(&buffer, data));

        GET_TOKEN();
    }


    if (data->token->token == TYPE_COMMA || data->token->token == TYPE_EOL || data->token->token == TYPE_EOF || (data->in_bracket == true && data->token->token == TYPE_RIGHT_BRACKET)) {
        insert_stop(&buffer);
    }

    res = handle_expression(data);
    if (res != EXPRESSION_OK) {
        return(res);
    }
    clear_buffer(&buffer);

    CHECK_GEN(gen_print())
    // ... , ...
    if (data->token->token == TYPE_COMMA) {
        // ... ID ...
        GET_TOKEN();
        // check next parameter
        IF_N_OK_RETURN(print(data));
        //return(print(data));
       // gen_print(); // if succes
    }

    // ... ) ... - optional
    if (data->in_bracket == true) {
        if (data->token->token != TYPE_RIGHT_BRACKET) {
            return(ER_SYN);
            
        }
        data->in_bracket = false;
        GET_TOKEN();
    }

    // ... EOL || EOF
    if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF) {
        return(SYN_OK);
    }

    return(ER_SYN);
}



static bool init_struct(Data_t* data){
    data->token = malloc(sizeof(Token_t));
    data->token->token = 100;
    data->in_bracket = false;
    data->in_while_or_if = 0;
    data->in_definition = false;
    data->in_declare = false;

    return true;
}


/*****************************************************************************************
 * ***************************************************************************************
 * PARSER
 * ***************************************************************************************
 *****************************************************************************************/
int start_parser(){

    Data_t our_data;
    string_t string;

    allocate_string(&string);
    set_string(&string);

    if(!init_struct(&our_data)){
        free_string(&string);
    }

    // initialization of buffer
    init_buffer(&buffer);

    //identifier = (char *)malloc(sizeof(char));
    allocate_string(&identifier);
    allocate_string(&identifier_f);
    allocate_string(&identifier_declare);

    // initialization of SymTable
    STinits();
    iteminit(&tItem, "",  NILL, false, 0);
    

    

    /*
    tstackP *s;
    FrameStackInit(s);
    */

    CHECK_GEN(gen_mainscope_start())

    int res;
    res = prog(&our_data);

    CHECK_GEN(gen_mainscope_end())


    if (res == SYN_OK) {
        res = STlast_check();
    }
    


    // clear buffer
    clear_buffer(&buffer);

    // delete SymTable
    htClearAlltables();
    itemfree(&tItem);
    
    free_string(&string);
    free(our_data.token);
    
    //free(identifier);
    free_string(&identifier);
    free_string(&identifier_f);
    free_string(&identifier_declare);

    free_stack(&stack);


    if (res == SYN_OK && our_data.in_while_or_if != 0)
        return(ER_SYN);
    else
        return res;
}







