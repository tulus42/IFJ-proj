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
//#include "symtable.h"
#include "instructions.h"


const char* tokens[] = {
	"TYPE_EOF", 
	"TYPE_EOL", 
	"TYPE_IDENTIFIER", 
	"TYPE_KEYWORD",

	"TYPE_ASSIGN", // =
	"TYPE_NEQ", // !=
	"TYPE_LEQ", // <=
	"TYPE_LTN", // <
	"TYPE_MEQ", // >=
	"TYPE_MTN", // >
	"TYPE_EQ", // ==
	
	"TYPE_PLUS", // +
	"TYPE_MINUS", //  -
	"TYPE_MUL", // *
	"TYPE_DIV", // /
	"TYPE_QUESTION_MARK", // ?
	"TYPE_COLON", // :

	"TYPE_LEFT_BRACKET", // (
	"TYPE_RIGHT_BRACKET", // )
	"TYPE_COMMA", // ,

	"TYPE_COMMENT", // #
	"TYPE_COMMENT_START", // =begin 
	"TYPE_COMMENT_END", // =end 

	"TYPE_INT", 
	"TYPE_FLOAT", 
	"TYPE_STRING",
};



#define GET_TOKEN()   \
    do {                                                       \
        if (get_next_token(data->token) != LEXER_OK)                             \
            return(ER_LEX); \
    } while (data->token->token == TYPE_COMMENT);

#define IS_VALUE()                                                           \
    data->token->token == TYPE_INT || data->token->token == TYPE_FLOAT      \
    || data->token->token == TYPE_STRING || data->token->token == TYPE_IDENTIFIER 

#define IF_N_OK_RETURN(__func__) \
    res = __func__; \
    if (res != SYN_OK) return(res);    

#define IS_OPERAND() \
    data->token->token == TYPE_PLUS ||data->token->token == TYPE_MINUS || data->token->token == TYPE_MUL \
    || data->token->token == TYPE_DIV || data->token->token == TYPE_NEQ || data->token->token == TYPE_LEQ \
    || data->token->token == TYPE_LTN || data->token->token == TYPE_MEQ || data->token->token == TYPE_MTN \
    || data->token->token == TYPE_EQ        

#define CHECK_BOTH_TABLES() \
    if (data->in_definition == true) { \
        if (check_define(local_ST, data->token->attr.string->s) != PARAM_DEFINED) { \
            return (ER_SEM_VARIABLE); \
        } \
    } else \
        if (check_define(global_ST, data->token->attr.string->s) != PARAM_DEFINED) { \
            return(ER_SEM_VARIABLE); \
    } 


/**
 * TODO: 
 * 
 * 
 * prerobit Errorove vystupy
 *    - if (something() != SYN_OK)
 *          return(NAVRATOVA HODNOTA of something());
 * 
 * insert_to_buffer - return value - vyriešiť napr makrom
 * 
 * generovanie - --------------||------------------ (opakovací symbol (ak by si nahodou nepochopil))
 */

// premenna pre uchovanie ID z tokenu pre neskorsie ulozenie do TS
string_t identifier_f;
string_t identifier;
string_t identifier_declare;

int res;
int params_cnt = 0;
int if_whlie_idx = 0;

Tmp_Token_t tmp;

tHTItem tItem;

// forward declarations
static int statement(Data_t* data);
static int declare(Data_t* data);
static int params(Data_t* data);
static int param(Data_t* data);
static int argvs(Data_t* data);
static int arg(Data_t* data);
static int value(Data_t* data);
static int function(Data_t* data);
static int print(Data_t* data); 


// Frees all the memory
int parser_error(Data_t* Data, string_t* string ){
    // frees the memory
    free_string(string);
    free(Data->token);

    return ER_SYN;
}

void save_id(string_t* identifier, Data_t* data) {
    add_string(identifier, data->token->attr.string->s);
}

// Kontrola uspesnosti lexikalnej analyzy
int get_token(Data_t* data) {
    if (get_next_token(data->token) == LEXER_OK)
        return(1);
    else
        return(0);
}

/* ***************************************************************************************
 * <prog>
 * ***************************************************************************************/
static int prog(Data_t* data){

    GET_TOKEN();

    //printf("In <prog>, Token: %s\n", tokens[data->token->token]);

    // <prog> -> DEF ID_FUNC ( <params> ) EOL <statement> END <prog>
    if(data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_DEF){
        if (data->in_definition == false) {
            data->in_definition = true;
        } else {
            return(ER_SYN);
        }

        // ... ID_FUNC ...
        GET_TOKEN();
        save_id(&identifier_f, data);
        //printf("ID: %s\n", identifier.s);
        
        gen_func_start((&identifier_f)->s);

        // ... ( ...
        GET_TOKEN();
        if (data->token->token != TYPE_LEFT_BRACKET) 
            return(ER_SYN);
        
        // ... <params> ...
        GET_TOKEN();
        params_cnt = 0;
        IF_N_OK_RETURN(params(data));
        
        // ... ) ... - nevolame GET_TOKEN
        if (data->token->token != TYPE_RIGHT_BRACKET) 
            return(ER_SYN);
    
        // ... EOL ...
        GET_TOKEN();
        if (data->token->token != TYPE_EOL) 
            return(ER_SYN);

        // priradenie ID_FUNC do tabulky
        itemupdate(&tItem, (&identifier_f)->s,  FUNCTION, true, params_cnt);
        res = htInsert(global_ST, &tItem);
        if (res != ST_OK) {
            return(res);
        }
        params_cnt = 0;

        //htPrintTable(global_ST);

        
        //printf("Checkpoint 1\n");

        // ... <statement> ...
        GET_TOKEN();
        IF_N_OK_RETURN(statement(data));
        
        //printf("Checkpoint 2\n");

        // ... END ...  - nevolame GET_TOKEN, pretoze sem sa vrati zo <statement> len ak uz token == END
        if (!(data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_END))
            return(ER_SYN);

        //printf("Checkpoint 3\n");

        // ... EOL || EOF ... 
        GET_TOKEN();
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF) {
            htClearlocal();
            return(prog(data));
        }
            
    } 


    // <prog> -> EOL <prog>
    else if(data->token->token == TYPE_EOL){
        //printf("Token in EOL loop: %d\n",data->token->token);
        return(prog(data));
    } 


    // <prog> -> EOF
    else if(data->token->token == TYPE_EOF){
        //printf("EOF - exit\n");
        return SYN_OK;
    } 


    // <prog> -> <statements> <prog>
    else if(data->token->token == TYPE_KEYWORD || data->token->token == TYPE_IDENTIFIER){
        //printf("INTO <statement> Token: %s\n", tokens[data->token->token]);
        int res=statement(data);
        //printf("Return value of <statement> is: %d\n", res);
        return res;
        //return(statement(data));  - pouzijeme toto, ale kvoli debuggingu je to rozpisane
    }

    // ... INT/FLT/STR ...
    else if (IS_VALUE()) {
        //printf("in <prog> IS VALUE\n");
        insert_to_buffer(&buffer, data);

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
static int statement(Data_t* data) {
    //printf("In <statement>, in_while_of_if: %d\n", data->in_while_or_if);
    //printf("Token: %s\n", tokens[data->token->token]);

    // <statement> -> IF <expression> THEN EOL <statements> ELSE EOL <statements> END EOL
    // ... IF ...
    if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_IF) {
        data->in_while_or_if++;
        if_whlie_idx +=2;
        int this_if = if_whlie_idx;
        // ... <expression> ...
        
        //printf("Check expression\n");
        res = handle_expression(data);
        if (res == EXPRESSION_OK) {                                   
            //printf("Spracoval som expression\n");
        } else 
            return(res);                     // TODO zmenit návratovú hodnotu       
        
        // ... THEN ...
        //printf("Check THEN\n");
        if (data->token->token != TYPE_KEYWORD || data->token->attr.keyword != KEYWORD_THEN) {
            //printf("ERR1\n");
            return(ER_SYN);
        }
    
        gen_if_start(this_if, data->in_while_or_if);
        // ... EOL ...
        GET_TOKEN();
        //printf("Check EOL\n");
        if (data->token->token != TYPE_EOL) {
            //printf("ERR2\n");
            return(ER_SYN);
        }

        // ... <statements> ...
        GET_TOKEN();
        //printf("Check next statement\n");
        IF_N_OK_RETURN(statement(data));
    
        // ... ELSE ... || ... EN rozsirenie - volitelna cast ELSE
        // ELSE
        //printf("Check ELSE\nToken: %s\n", tokens[data->token->token]);
        if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_ELSE) {
            // ... EOL ...
            GET_TOKEN();
            //printf("Check EOL\n");
            if (data->token->token != TYPE_EOL) {
                //printf("ERR4\n");
                return(ER_SYN);
            }
          
            gen_if_else(this_if, data->in_while_or_if);
            // ... <statements> ...
            GET_TOKEN();
            //printf("Check next statement\n");
            IF_N_OK_RETURN(statement(data));
        }

        else
        { gen_if_else(this_if, data->in_while_or_if+1);}

        
        
        // ... END ...  - nevolame get_token(), pretoze sem sa vrati zo <statement> len ak uz token == END
        if (!(data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_END))
            return(ER_SYN);

        gen_if_end(this_if+1, data->in_while_or_if+1);

        // ... EOL || EOF ... 
        GET_TOKEN();
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF)   
            return(prog(data));
        

        // if nothing good after IF, return ERROR  
        //printf("ERR7\n");
        return(ER_SYN);
    }
    

    // <statement> -> WHILE <expression> DO EOL <statement> END EOL
    // ... WHILE ...
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_WHILE) {
        data->in_while_or_if++;
        if_whlie_idx +=2;
        int this_while = if_whlie_idx;

        gen_while_header(this_while, data->in_while_or_if);
        // ... <expression> ...
        res = handle_expression(data);
        if (res == EXPRESSION_OK) {      // TODO expression
            //printf("Spracoval som expression\n");
        } else 
            return(res);
        
        // ... DO ...
        if (data->token->token != TYPE_KEYWORD || data->token->attr.keyword != KEYWORD_DO)
            return(ER_SYN);
    
        // ... EOL ...
        GET_TOKEN();
        if (data->token->token != TYPE_EOL)
            return(ER_SYN);

       
        gen_while_start(this_while+1, data->in_while_or_if);
        
        // ... <statements> ...
        GET_TOKEN();
        //printf("Check next statement\n");
        IF_N_OK_RETURN(statement(data));
        
        // ... END ...  - nevolame get_token(), pretoze sem sa vrati zo <statement> len ak uz token == END
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
    // pre vstavené funkcie
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword > 8 && data->token->attr.keyword < 17) {
        //printf("in <statement> -> <function>\n");
        // ... <function> ...
        IF_N_OK_RETURN(function(data));

        // ... EOL || EOF ... - token nepytame, bude vdaka nemu navratena <function>
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF)   
            return(prog(data));
    }
    

    // <statement> -> ID EOL
    // <statement> -> ID <declare> EOL
    // <statement> -> ID_FUNC ( <argvs> ) EOL
    else if (data->token->token == TYPE_IDENTIFIER) {


        //printf("in <statement> ID EOL/ID <declare>/ID_FUNC...\n");
        save_id(&identifier, data);
        save_id(&identifier_declare, data);
        

        GET_TOKEN();

        // <statement> -> ID_FUNC 
        if (check_define(global_ST, (&identifier)->s) == FUNCTION_DEFINED) {
            
            gen_func_prep_for_params();

            IF_N_OK_RETURN(argvs(data));

            gen_func_call((&identifier)->s);

            return(prog(data));

        }


        // <statement> -> ID EOL || EOF 
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF) {
            
            itemupdate(&tItem, (&identifier)->s,  VAR, false, 0);

            // ak sme v DEF
            if (data->in_definition == true) {
                if (check_define(local_ST, identifier_declare.s) != PARAM_DEFINED) {
                    gen_var_declar(identifier_declare.s);
                    gen_var_defval((&identifier)->s);
                }


                res = htInsert(local_ST, &tItem);
                //printf("idetmInsert returned: %d\n", res);
                if (res != ST_OK) {
                    return(res);
                }
                //htPrintTable(local_ST);
            
            // ak sme na globalnej urovni
            } else {
                if (check_define(global_ST, identifier_declare.s) != PARAM_DEFINED) {
                    gen_var_declar(identifier_declare.s);
                    gen_var_defval((&identifier)->s);
                }

                res = htInsert(global_ST, &tItem);

                

                //printf("idetmInsert returned: %d\n", res);
                if (res != ST_OK) {
                    return(res);
                }
                //htPrintTable(global_ST);
            }
            
            
            

            return(prog(data));
        } else



        // <statement> -> ID <declare> EOL
        // ... = ...
        if (data->token->token == TYPE_ASSIGN) {
            if (data->in_definition == true) {
                if (check_define(local_ST, identifier_declare.s) != PARAM_DEFINED) {
                    gen_var_declar(identifier_declare.s);
                }
            } else {
                if (check_define(global_ST, identifier_declare.s) != PARAM_DEFINED) {
                    gen_var_declar(identifier_declare.s);
                }
            }
            
            res = declare(data);
            if (res== SYN_OK) {
                itemupdate(&tItem, (&identifier_declare)->s, VAR, true, 0);
                

                if (data->in_definition == true) {
                    res = htInsert(local_ST, &tItem);
                    //printf("idetmInsert returned: %d\n", res);
                    if (res != ST_OK) {
                        return(res);
                    }
                    //htPrintTable(local_ST);

                } else {
                    res = htInsert(global_ST, &tItem);
                    //printf("itemInsert returned: %d\n", res);
                    if (res != ST_OK) {
                        return(res);
                    }
                    //htPrintTable(global_ST);
                }
            } else {
                return(res);
            }
            return(prog(data));
        } else

        
        
        // VOLANIE FUNKCIE PRED JEJ DEFINICIOU
        // else if (ID_func) then :************************
        // <statement> -> ID_FUNC ( <argvs> )
        if (data->token->token == TYPE_LEFT_BRACKET || IS_VALUE()) {
            //printf("in <volanie funkcie pred deklaraciou>\n");
            /******
             * check if ID_FUNC in table
             * if (in table) {
             *    vsetko ok a pokracuj
             * } else {
             *    if (data->in_declaration == true) {
             *       save ID_FUNC in table, ID_FUNC.declared = false;
             *    } else
             *       return(ER_SYN);
             * }
             */

            if (check_define(global_ST, (&identifier)->s) != FUNCTION_DEFINED && check_define(global_ST, (&identifier)->s) != PARAM_DEFINED && data->in_definition == true) {
                IF_N_OK_RETURN(argvs(data));
                //printf("Checkpoint 42\n");

                itemupdate(&tItem, (&identifier)->s, FUNCTION, false, params_cnt);
                res = htInsert(global_ST, &tItem);
                if (res != ST_OK) {
                    return(res);
                }
                //htPrintTable(global_ST);
            } else {
                return(ER_SYN);
            }

            return(prog(data));

        } else {

            return(ER_SYN);
        }
    }

    

    // <statement> -> EOL <prog>
    // povoluje brat prazdne riadky ako statement - napr IF () THEN 2xEOL END
    if (data->token->token == TYPE_EOL)
        return(prog(data));

    // ****************************************
    // make <statement> return when END or ELSE
    if (data->token->token == TYPE_KEYWORD && (data->token->attr.keyword == KEYWORD_END || data->token->attr.keyword == KEYWORD_ELSE)) {
        // ak ide o IF/WHILE
        if (data->in_while_or_if != 0) {
            //printf("Check in_while_or_if\n");
            if (data->token->attr.keyword == KEYWORD_END) {

                data->in_while_or_if--;
                //printf("Return <statement>2\n");
                return(SYN_OK);
            }
            if (data->token->attr.keyword == KEYWORD_ELSE) {
                //printf("Return <statement>1\n");
                return(SYN_OK);
            }
        } else 
        // ak ide o DEFINITION
        if (data->in_definition == true) {
            if (data->token->attr.keyword == KEYWORD_END) {
                data->in_definition = false;

                gen_func_rval();
                gen_func_ret((&identifier_f)->s);
                gen_func_end((&identifier_f)->s);

                //printf("Return <statement>3\n");
                return(SYN_OK);
            }
        } else
            return(ER_SYN);
    }



    //printf("End <statemtnt> with ERR\n");
    return(ER_SYN);
}



/* ***************************************************************************************
 * <declare>
 * ***************************************************************************************/
static int declare(Data_t* data) {
    // <declare> =
    data->in_declare = true;

    GET_TOKEN();

    // ak ID, ID_FUNC, int/flt/str
    if (IS_VALUE()) {
        insert_to_buffer(&buffer, data);

        // ... ID, ID_FUNC ...
        if (data->token->token == TYPE_IDENTIFIER) {
            // TODO - pripad, ze sme v definici
            save_id(&identifier, data);
            //printf("ID1: %s\nID2: %s\nID3: %s\n", identifier_f.s,identifier.s,identifier_declare.s);

            // ... ID_FUNC ...
            if (check_define(global_ST, data->token->attr.string->s) == FUNCTION_DEFINED) {
                
                gen_func_prep_for_params();
                GET_TOKEN();

                IF_N_OK_RETURN(argvs(data));
                //itemupdate(&tItem, (&identifier_declare)->s, VAR, true, 0);
                ////printf("ID1: %s\nID2: %s\n", identifier_f.s,identifier.s);
                //if (data->in_definition == true) {
                //    htInsert(local_ST, &tItem);    
                //} else {
                //    htInsert(global_ST, &tItem);
                //}
                
                
                gen_func_call((&identifier)->s);
                gen_func_rval_assign((&identifier_declare)->s);
                //printf("ID1: %s\nID2: %s\n", identifier_f.s,identifier.s);

                clear_buffer(&buffer);
                data->in_declare = false;
                return(SYN_OK);

            } else

            // ... ID ... 
            if ((data->in_definition == false && check_define(global_ST, identifier.s) == PARAM_DEFINED) ||
                (data->in_definition == true && check_define(local_ST, identifier.s) == PARAM_DEFINED)) {
                
                res = handle_expression(data);
                if (res != EXPRESSION_OK) {
                    clear_buffer(&buffer);
                    return(res);
                }

                gen_assign(identifier_declare.s);

                clear_buffer(&buffer);
                data->in_declare = false;
                return(SYN_OK);


                /*
                GET_TOKEN();


                // ak nasleduje operand, vyhodnosti expression
                if (IS_OPERAND()) {
                    
                    res = handle_expression(data);
                    if (res != EXPRESSION_OK) {
                        clear_buffer(&buffer);
                        return(res);
                    }

                    gen_assign(identifier_declare.s);

                    clear_buffer(&buffer);
                    data->in_declare = false;
                    return(SYN_OK);

                } else

                // ... EOL || EOF ...
                if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF) {
                    insert_stop(&buffer);
                    
                    res = handle_expression(data);
                    if (res != EXPRESSION_OK) {
                        return(res);
                    }
                    clear_buffer(&buffer);

                    gen_assign(identifier_declare.s);
                    data->in_declare = false;
                    return(SYN_OK);
                }*/

            // ak ID nie je definovane -> ERR
            } else {
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

            gen_assign(identifier_declare.s);

            clear_buffer(&buffer);
            data->in_declare = false;
            return(SYN_OK);

            /*
            // ak nasleduje operand, vyhodnosti expression
            if (IS_OPERAND()) {
                insert_to_buffer(&buffer, data);
                res = handle_expression(data);
                if (res != EXPRESSION_OK) {
                    clear_buffer(&buffer);
                    return(res);
                }
                clear_buffer(&buffer);
                data->in_declare = false;
                gen_assign(identifier_declare.s);
                return(SYN_OK);

            } else

            // ... EOL || EOF ...
            if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF) {
                insert_stop(&buffer);
                    
                res = handle_expression(data);
                if (res != EXPRESSION_OK) {
                    return(res);
                }
                clear_buffer(&buffer);

                gen_assign(identifier_declare.s);
                data->in_declare = false;
                return(SYN_OK);
            }*/
        }

        clear_buffer(&buffer);


        return(SYN_OK);
    } else 

    if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword > 8 && data->token->attr.keyword < 17) {
        
        res = function(data);
        if (res != SYN_OK) {
            return(res);
        } else {
            data->in_declare = false;
            return(SYN_OK);
        }
    } else 

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
static int params(Data_t* data) {
    // <params> -> ID <param>
    // ... ID ...

    if (data->token->token == TYPE_IDENTIFIER) {
        //printf("ID: %s\n",data->token->attr.string->s);
        int res;
        res = param(data);

        //htPrintTable(local_ST);
        //printf("<params> return: %d\n", res);
        return(res);
        //return(param(data));
    } else

    // <params> -> ε
    // ... ) ... - nepytame TOKEN - len pripad f()
    if (data->token->token == TYPE_RIGHT_BRACKET) {
        return(SYN_OK);
    }

    return(ER_SYN);
}

static int param(Data_t* data) {
    // <param> , ID <param>
    // ... ID ... - znovu musime kontrolovat ID, kvoli rekurzivnemu volaniu
    if (data->token->token == TYPE_IDENTIFIER) {
        //printf("<param> ID: %s\n", data->token->attr.string->s);
    /**
     * somehow check ID in table
     * 
     **/
        params_cnt++;
        save_id(&identifier, data);

        gen_func_param((&identifier)->s, params_cnt);

        res = def_ID(local_ST, (&identifier)->s);
        if (res != ST_OK) {
            return(res);
        }


        GET_TOKEN();
        // ... , ...
        if (data->token->token == TYPE_COMMA) {

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
static int argvs(Data_t* data) {
    // <argvs> -> <value> <arg>
    // ... <value> ...

    params_cnt = 0;

    //printf("in <argvs>\n");
     // ... ( ... - volitelna
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
static int arg(Data_t* data) {
    // <arg> -> , <value> <arg>
    // <arg> -> ε

    //printf("in <arg>\n");
    

    if (IS_VALUE()) {
        params_cnt++;
        //printf("in IS_VALUE\n");
    
        if (data->token->token == TYPE_IDENTIFIER) {
            //printf("ID: %s\n", data->token->attr.string->s);
            if (data->in_definition == true) {
                if (check_define(local_ST, data->token->attr.string->s) != PARAM_DEFINED) {
                    return(ER_SEM_VARIABLE);
                }
            } else {
                if (check_define(global_ST, data->token->attr.string->s) != PARAM_DEFINED) {
                    //printf("TU SOM SPADOL\n");
                    return(ER_SEM_VARIABLE);
                }
            }
        }

        gen_func_pass_param(*(data->token), params_cnt);
        //printf("in arg Checkpoint\n");
        GET_TOKEN();
        // ... , ...
        if (data->token->token == TYPE_COMMA) {

            GET_TOKEN();
            return(arg(data));
        
        } else {
            // ... ) ... - len ak bola pouzita "("
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
                //printf("in arg Checkpoint2\n");
                return(SYN_OK);
            }
        }
        GET_TOKEN();
    }

    
    
        
        // ... ) ... - len ak bola pouzita "("
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
 * <value>
 * ***************************************************************************************/

static int value(Data_t* data) {
    // <value> -> INT_VALUE
    if (data->token->token == TYPE_INT){

    }
    // <value> -> FLOAT_VALUE
    if (data->token->token == TYPE_FLOAT){

    }
    // <value> -> STRING_VALUE
    if (data->token->token == TYPE_STRING){

    }

    return(ER_SYN);
}


/* ***************************************************************************************
 * <function>
 * ***************************************************************************************/
static int function(Data_t* data) {
    //printf("in <function>\n");

    // <function> -> PRINT ( <argvs> ) EOL
    if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_PRINT) {
        //printf("in <function> PRINT\n");

        GET_TOKEN();

        // ... ( ... - volitelna
        if (data->token->token == TYPE_LEFT_BRACKET) {
            data->in_bracket = true;
            GET_TOKEN();
        }

        clear_buffer(&buffer);
        // nenulovy pocet argumentov - ak by nasledovala ")" alebo EOL/EOF -> ER_SYN
        if (IS_VALUE()) {
            IF_N_OK_RETURN(print(data));


            // ... EOL || EOF ... - uz skontrolovany token v PRINT
            return(SYN_OK);
        } else
        return(ER_SYN);
    }


    // <function> -> LENGTH ( <argvs> ) EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_LENGTH) {
        //printf("in <function> LENGTH\n");

        gen_func_prep_for_params(); 
        
        GET_TOKEN();

        // ... ( ... - volitelna
        if (data->token->token == TYPE_LEFT_BRACKET) {
            data->in_bracket = true;
            GET_TOKEN();
        }

        // ... string ...
        if (data->token->token == TYPE_STRING) {
            // vygeneruj instrukciu pre dlzku stringu

        } else 

        // ... ID ...
        if (data->token->token == TYPE_IDENTIFIER) {
            // check ID in table 
            CHECK_BOTH_TABLES();
        
        } 
        
        
        // ak cokolvek ine -> nevyhovujuci parameter
        else {
            return(ER_SEM_TYPE);
        }
        gen_func_pass_param(*(data->token), 0);

        GET_TOKEN();
        
        // ... ) ... - len ak bola pouzita "("
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
            gen_func_call("length");
            if (data->in_declare == true) {
                gen_func_rval_assign(identifier_declare.s);
            }
            return(SYN_OK);
        }

        return(ER_SYN);
    }


    // <function> -> SUBSTR ( s, i, n ) EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_SUBSTR) {
        //printf("in <function> SUBSTR\n");

        gen_func_prep_for_params(); 
        
        GET_TOKEN();

        // ... ( ... - volitelna
        if (data->token->token == TYPE_LEFT_BRACKET) {
            data->in_bracket = true;
            GET_TOKEN();
        }

        /***
         * PRVY PARAMETER - s
         ***/
        // ... string ...
        if (data->token->token == TYPE_STRING) {
            // vyhovuje

        } else 

        // ... ID ...
        if (data->token->token == TYPE_IDENTIFIER) {
            // check ID in table 
            CHECK_BOTH_TABLES();
        
        } 
        
        // ak cokolvek ine -> nevyhovujuci parameter
        else {
            return(ER_SEM_TYPE);
        }
        gen_func_pass_param(*(data->token), 0);

        GET_TOKEN();

        // ... , ...
        if (data->token->token == TYPE_COMMA) {
            GET_TOKEN();
        } else {
            return(ER_SYN);
        }


        /***
         * DRUHY PARAMETER - i
         ***/
        // ... int ...
        if (data->token->token == TYPE_INT) {
            // vyhovuje

        } else 

        // ... ID ...
        if (data->token->token == TYPE_IDENTIFIER) {
            // check ID in table
            CHECK_BOTH_TABLES();
        
        } 
        
        // ak cokolvek ine -> nevyhovujuci parameter
        else {
            return(ER_SEM_TYPE);
        }
        gen_func_pass_param(*(data->token), 1);
        GET_TOKEN();

        // ... , ...
        if (data->token->token == TYPE_COMMA) {
            GET_TOKEN();
        } else {
            return(ER_SYN);
        }


        /***
         * TRETI PARAMETER - n
         ***/
        // ... int ...
        if (data->token->token == TYPE_INT) {
            // vyhovuje

        } else 

        // ... ID ...
        if (data->token->token == TYPE_IDENTIFIER) {
            // check ID in table 
            CHECK_BOTH_TABLES();
        
        } 
        
        // ak cokolvek ine -> nevyhovujuci parameter
        else {
            return(ER_SEM_TYPE);
        }

        gen_func_pass_param(*(data->token), 2);
        GET_TOKEN();


        // ... ) ... - len ak bola pouzita "("
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
            gen_func_call("substr");
            if (data->in_declare == true) {
                gen_func_rval_assign(identifier_declare.s);
            }
            return(SYN_OK);
        }

        return(ER_SYN);
    }


    // <function> -> ORD ( s, i ) EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_ORD) {
        //printf("in <function> ORD\n");

        gen_func_prep_for_params(); 
        
        GET_TOKEN();

        // ... ( ... - volitelna
        if (data->token->token == TYPE_LEFT_BRACKET) {
            data->in_bracket = true;
            GET_TOKEN();
        }

        /***
         * PRVY PARAMETER - s
         ***/
        // ... string ...
        if (data->token->token == TYPE_STRING) {
            // vyhovuje

        } else 

        // ... ID ...
        if (data->token->token == TYPE_IDENTIFIER) {
            // check ID in table 
            CHECK_BOTH_TABLES();
        
        } 
        
        // ak cokolvek ine -> nevyhovujuci parameter
        else {
            return(ER_SEM_TYPE);
        }

        gen_func_pass_param(*(data->token), 0);

        GET_TOKEN();

        // ... , ...
        if (data->token->token == TYPE_COMMA) {
            GET_TOKEN();
        } else {
            return(ER_SYN);
        }


        /***
         * DRUHY PARAMETER - i
         ***/
        // ... int ...
        if (data->token->token == TYPE_INT) {
            // vyhovuje

        } else 

        // ... ID ...
        if (data->token->token == TYPE_IDENTIFIER) {
            // check ID in table
            CHECK_BOTH_TABLES();

        } 
        
        // ak cokolvek ine -> nevyhovujuci parameter
        else {
            return(ER_SEM_TYPE);
        }

        gen_func_pass_param(*(data->token), 1);

        GET_TOKEN();


        // ... ) ... - len ak bola pouzita "("
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
            gen_func_call("ord");
            if (data->in_declare == true) {
                gen_func_rval_assign(identifier_declare.s);
            }
            return(SYN_OK);
        }

        return(ER_SYN);
    }


    // <function> -> CHR ( i ) EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_CHR) {
        //printf("in <function> CHR\n");

        gen_func_prep_for_params(); 
       
        GET_TOKEN();

        // ... ( ... - volitelna
        if (data->token->token == TYPE_LEFT_BRACKET) {
            data->in_bracket = true;
            GET_TOKEN();
        }

        // ... int ...
        if (data->token->token == TYPE_INT) {
            // vyhovuje

        } else 

        // ... ID ...
        if (data->token->token == TYPE_IDENTIFIER) {
            // check ID in table - musi byt typu int
            CHECK_BOTH_TABLES();
        } 
        
        // ak cokolvek ine -> nevyhovujuci parameter
        else {
            return(ER_SEM_TYPE);
        }

        gen_func_pass_param(*(data->token), 0);

        GET_TOKEN();


        // ... ) ... - len ak bola pouzita "("
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
            gen_func_call("chr");
            if (data->in_declare == true) {
                gen_func_rval_assign(identifier_declare.s);
            }
            return(SYN_OK);
        }

        return(ER_SYN);

    }


    // <function> -> INPUTS EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_INPUTS) {
        //printf("in <function> INPUTS\n");
    
        GET_TOKEN();

        // ... ( ... - volitelna
        if (data->token->token == TYPE_LEFT_BRACKET) {
            GET_TOKEN();

            // ... ) ...
            if (data->token->token == TYPE_RIGHT_BRACKET) {
                GET_TOKEN();
            } else {
                return(ER_SYN);
            }
        }

        // true == priradzujeme funkciu premennej  TODO
        gen_input((&identifier)->s, STRING, data->in_declare);

        // ... EOL || EOF ...
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF) {
            data->in_declare = false;
            return(SYN_OK);
        }

        return(ER_SYN);
    }

    
    // <function> -> INPUTI EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_INPUTI) {
        //printf("in <function> INPUTI\n");
    
        GET_TOKEN();

        // ... ( ... - volitelna
        if (data->token->token == TYPE_LEFT_BRACKET) {
            GET_TOKEN();

            // ... ) ...
            if (data->token->token == TYPE_RIGHT_BRACKET) {
                GET_TOKEN();
            } else {
                return(ER_SYN);
            }
        }

        gen_input((&identifier)->s, INTEGER, data->in_declare);

        // ... EOL || EOF ...
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF) {
            data->in_declare = false;
            return(SYN_OK);
        }

        return(ER_SYN);
    }


    // <function> -> INPUTF EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_INPUTF) {
        //printf("in <function> INPUTF\n");
    
        GET_TOKEN();

        // ... ( ... - volitelna
        if (data->token->token == TYPE_LEFT_BRACKET) {
            GET_TOKEN();

            // ... ) ...
            if (data->token->token == TYPE_RIGHT_BRACKET) {
                GET_TOKEN();
            } else {
                return(ER_SYN);
            }
        }

        gen_input((&identifier)->s, PRASATKO_S_PAPUCKAMI_FLT, data->in_declare);
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
 * je rozpisany ako samostatna funkcia kvoli lubovolnemu poctu parametrov
 * pri kazdom dalsom parametri je rekurzivne volana znovu
 * ***************************************************************************************/
static int print(Data_t* data) {
    //printf("in PRINT\n");

    clear_buffer(&buffer);
    while (data->token->token != TYPE_COMMA && data->token->token != TYPE_EOL && data->token->token != TYPE_EOF) {
        if (data->in_bracket == true && data->token->token == TYPE_RIGHT_BRACKET) {
            break;
        }
        //printf("<pritn> in while\n");
        
        insert_to_buffer(&buffer, data);

        GET_TOKEN();
    }


    /*if (data->token->token == TYPE_COMMA || data->token->token == TYPE_EOL || data->token->token == TYPE_EOF || (data->in_bracket == true && data->token->token == TYPE_RIGHT_BRACKET)) {
        insert_stop(&buffer);
    }*/

    if (data->token->token == TYPE_COMMA) {
        insert_stop(&buffer);
    }

    if (data->token->token == TYPE_EOL) {
        insert_stop(&buffer);
    }

    if (data->token->token == TYPE_EOF) {
        insert_stop(&buffer);
    }

    if (data->in_bracket == true && data->token->token == TYPE_RIGHT_BRACKET) {
        insert_stop(&buffer);
    }



    /*
    // ... ID ...
    if (data->token->token == TYPE_IDENTIFIER) {
        // check ID in table

        CHECK_BOTH_TABLES();
    } else

    // ... INT/FLT/STR ...
    if (IS_VALUE()) {
        
    }

    */
    //printf("into epression\n");

    res = handle_expression(data);
    if (res != EXPRESSION_OK) {
        return(res);
    }
    clear_buffer(&buffer);

    gen_print();
    // ... , ...
    if (data->token->token == TYPE_COMMA) {
        // ... ID ...
        GET_TOKEN();
        IF_N_OK_RETURN(print(data));
        //return(print(data));
       // gen_print(); // if succes
    }

    // ... ) ... - volitelna
    if (data->in_bracket == true) {
        if (data->token->token != TYPE_RIGHT_BRACKET) {
            return(ER_SYN);
            
        }
        data->in_bracket = false;
        GET_TOKEN();
    }

    // ... EOL || EOF
    if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF) {
        //printf("end PRINT success\n");
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

    // inicializacia bufferu
    init_buffer(&buffer);

    //identifier = (char *)malloc(sizeof(char));
    allocate_string(&identifier);
    allocate_string(&identifier_f);
    allocate_string(&identifier_declare);

    // inicializacia tabulky symbolov
    STinits();
    iteminit(&tItem, "",  NILL, false, 0);
    

    

    /*
    tstackP *s;
    FrameStackInit(s);
    */

    gen_mainscope_start();

    int res;
    res = prog(&our_data);

    gen_mainscope_end();

    //printf("res = %d\n", res);
    value(&our_data);

    if (res == SYN_OK) {
        res = STlast_check();
    }
    


    // odstránenie bufferu
    clear_buffer(&buffer);

    // odstránenie tabulky
    htClearAlltables();
    itemfree(&tItem);
    
    free_string(&string);
    free(our_data.token);

    //free(identifier);
    free_string(&identifier);
    free_string(&identifier_f);
    free_string(&identifier_declare);

    /*
    DeleteStack(s);
    */

    //printf("EXIT code: %d\n", res);

    if (our_data.in_while_or_if != 0)
        return(ER_SYN);
    else
        return res;
    //return 0;
}







