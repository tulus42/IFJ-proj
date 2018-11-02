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
#include "symtable.h"

#include "lexer.c"
#include "parser.h"


/**
 * TODO: 
 * nový main
 * kontrola 0 alebo 1 LEX. analýzy
 * allocation of tokens
 * structures for parser.h
 * procedenční tabuľka
 * LL-gramatika
 */

// forward declarations
static int statement(Data_t* data);
static int declare(Data_t* data);
static int params(Data_t* data);
static int param(Data_t* data);
static int argvs(Data_t* data);
static int arg(Data_t* data);
static int value(Data_t* data);


// Frees all the memory
int parser_error(Data_t* Data, string_t* string ){
    // frees the memory
    free_string(string);
    free(Data->token);

    return ER_SYN;
}



// Kontrola uspesnosti lexikalnej analyzy
int get_token(Data_t* data) {
    if (get_next_token(data->token) == LEXER_OK)
        return(1);
    else
        return(0);
}

/* ****************************
 * <prog>
 * ***************************/
static int prog(Data_t* data){
    int result;

    if(get_token(data)) {
        printf("In <prog>\nToken: %d\n",data->token->token);
        // <prog> -> DEF ID_FUNC ( <params> ) EOL <statement> END <prog>
        if(data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_DEF){
        } // <prog> -> EOL <prog>
        else if(data->token->token == TYPE_EOL){
            //if(get_token(data)) {
                printf("Token: %d\n",data->token->token);
                return(prog(data));
            //} else {
            //    return ER_LEX;
            //}
        } // <prog> -> EOF
        else if(data->token->token == TYPE_EOF){
            printf("EOF - exit\n");
            exit(SYN_OK);
            //return SYN_OK;
        } // <prog> -> <statements> <prog> ???? toto treba ešte domyslieť
        else if(data->token->token == TYPE_KEYWORD || data->token->token == TYPE_IDENTIFIER){
            //if(get_token) {
                printf("INTO <statement> Token: %d\n",data->token->token);
                int res=statement(data);
                printf("Return value of <statement> is: %d\n", res);
                return res;
                return(statement(data));
            //}
            //else{
            //    return ER_LEX;
            //}
        }
    } else 
        return ER_LEX;
    
    return ER_SYN;
}

/* ****************************
 * <statement>
 * ***************************/
static int statement(Data_t* data) {
    printf("In <statement>, in_while_of_if: %d\n", data->in_while_or_if);

    // <statement> -> IF <expression> THEN EOL <statements> ELSE EOL <statements> END EOL
    // ... IF ...
    if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_IF) {
        data->in_while_or_if = true;
        // ... <expression> ...
        if (get_token(data)) {
            printf("Check expression\n");
            if (1/*__expression__*/) {      // TODO expression
                ;
            } else 
                return(ER_SYN);
        } else
            return(ER_LEX); 

        // ... THEN ...
        if (get_token(data)) {
            printf("Check THEN\n");
            if (data->token->token != TYPE_KEYWORD || data->token->attr.keyword != KEYWORD_THEN) {
                printf("ERR\n");
                return(ER_SYN);
            }
        } else
            return(ER_LEX);

        // ... EOL ...
        if (get_token(data)) {
            printf("Check EOL\n");
            if (data->token->token != TYPE_EOL) {
                printf("ERR\n");
                return(ER_SYN);
            }
        } else 
            return(ER_LEX);

        // ... <statements> ...
        if (get_token(data)) {
            printf("Check next statement\n");
            if (!statement(data)){
                printf("ERR\n");
                return(ER_SYN);
            }
        } else 
            return(ER_LEX);

        // ... ELSE ... || ... END ... - rozsirenie - volitelna cast ELSE
        if (get_token(data)) {
            // ELSE
            printf("Check ELSE\n");
            if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_ELSE) {
                // ... EOL ...
                if (get_token(data)) {
                    printf("Check EOL\n");
                    if (data->token->token != TYPE_EOL) {
                        printf("ERR\n");
                        return(ER_SYN);
                    }
                } else 
                    return(ER_LEX);
                
                // ... <statements> ...
                if (get_token(data)) {
                    printf("Check next statement\n");
                    if (!statement(data)){
                        printf("ERR\n");
                        return(ER_SYN);
                    }
                } else 
                    return(ER_LEX);
                
                if (!get_token(data))
                    return(ER_LEX);
            }
            // END
            printf("Check END\n");
            if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_END) {
                // ... EOL ...
                if (get_token(data)) {
                    printf("Check EOL\n");
                    if (data->token->token == TYPE_EOL) 
                        return(prog(data));
                    else{
                        printf("ERR\n");
                        return(ER_SYN);
                    }
                } else 
                    return(ER_LEX);
            }

        } else 
            return(ER_LEX);

        if (get_token(data))
            return(prog(data));
        else
            return(ER_LEX);
    }
    
    // <statement> -> WHILE <expression> DO EOL <statement> END EOL
    // ... WHILE ...
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_WHILE) {
        // ... <expression> ...
        if (get_token(data)) {
            if (1/*__expression__*/) {      // TODO expression
                ;
            } else 
                return(ER_SYN);
        } else
            return(ER_LEX); 
    
        // ... DO ...
        if (get_token(data)) {
            if (data->token->token != TYPE_KEYWORD || data->token->attr.keyword != KEYWORD_DO)
                return(ER_SYN);
        } else 
            return(ER_LEX);

        // ... EOL ...
        if (get_token(data)) {
            if (data->token->token != TYPE_EOL)
                return(ER_SYN);
        } else 
            return(ER_LEX);
        
        // ... <statements> ...
        if (get_token(data)) {
            printf("Check next statement\n");
            if (!statement(data))
                return(ER_SYN);
        } else 
            return(ER_LEX);


        if (get_token(data)) {

        } else
            return(ER_LEX);
        
    }

    if (data->token->token == TYPE_EOL)
        return(prog(data));

    if (data->token->token == TYPE_KEYWORD && (data->token->attr.keyword == KEYWORD_END || data->token->attr.keyword == KEYWORD_ELSE)) {
        if (data->in_while_or_if == true) {
            printf("Check in_while_or_if\n");
            if (data->token->attr.keyword == KEYWORD_END) {

                data->in_while_or_if = false;
                printf("Return <statement>\n");
                return 0;
            }
            if (data->token->attr.keyword == KEYWORD_ELSE) {
                printf("Return <statement>\n");
                return 0;
            }
        }
    }
    // <statement> -> PRINT ( <argvs> ) EOL
    // <statement> -> LENGTH ( <argvs> ) EOL
    // <statement> -> SUBSTR ( <argvs> ) EOL
    // <statement> -> ORD ( <argvs> ) EOL
    // <statement> -> CHR ( <argvs> ) EOL
    // <statement> -> INPUTS EOL
    // <statement> -> INPUTI EOL
    // <statement> -> INPUTF EOL
    // <statement> -> ID <declare> EOL

    printf("End <statemtnt>\n");
}

static int declare(Data_t* data) {

}

static int params(Data_t* data) {

}

static int param(Data_t* data) {

}

static int argvs(Data_t* data) {

}

static int arg(Data_t* data) {

}

static int value(Data_t* data) {

}



static bool init_struct(Data_t* data){
    data->token = malloc(sizeof(Token_t));
    data->in_function = false;
    data->in_while_or_if = false;

    return true;
}

int start_parser(){

    Data_t our_data;
    string_t string;

    allocate_string(&string);
    set_string(&string);

    if(!init_struct(&our_data)){
        free_string(&string);
    }
    
    
    
    //int lex_result = get_next_token(our_data.token);
    int res;
    //if(lex_result == 0){
        
        res = prog(&our_data);
        //prog(&our_data);
    //}
    //else{
    //    res = ER_LEX;
        //return parser_error(&our_data, &string);
    //}
    
    free_string(&string);
    free(our_data.token);
    return res;
    //return 0;
}







