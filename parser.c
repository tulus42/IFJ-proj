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
static int statement(struct Data_t* data);
static int declare(struct Data_t* data);
static int params(struct Data_t* data);
static int param(struct Data_t* data);
static int argvs(struct Data_t* data);
static int arg(struct Data_t* data);
static int value(struct Data_t* data);


// Frees all the memory
int parser_error(Token_t* token, struct Data_t* Dataptr){
    // frees the memory
    free_string(token->attr.string);
    free(token);
    free(Dataptr);

    return ER_SYN;
}

// Kontrola uspesnosti lexikalnej analyzy
int get_token(struct Data_t* data) {
    if (get_next_token(data->token) == LEXER_OK)
        return(1);
    else
        return(0);
}


static int prog(struct Data_t* data){
    int result;

    // <prog> -> DEF ID_FUNC ( <params> ) EOL <statement> END <prog>
    if(data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_DEF){

    } // <prog> -> EOL <prog>
    else if(data->token->token == TYPE_EOL){
        if(get_token)
            return(prog(data));
        else{
            return ER_LEX;
        }
    } // <prog> -> EOF
    else if(data->token->token == TYPE_EOF){
        return SYN_OK;
    } // <prog> -> <statements> <prog> ???? toto treba ešte domyslieť
    else if(data->token->token == TYPE_KEYWORD || data->token->token == TYPE_IDENTIFIER){
        if(get_token) {
            return(statement(data));
        }
        else{
            return ER_LEX;
        }
    }

    return ER_SYN;
}

static int statement(struct Data_t* data) {
    // <statement> -> IF <expression> THEN EOL <statements> ELSE EOL <statements> END EOL
    if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_IF) {
        // ... <expression> ...
        if (get_token(data)) {
            if (1/*__expression__*/) {      // TODO expression
                ;
            } else 
                return(ER_SYN);
        } else
            return(ER_LEX); 

        // ... THEN ...
        if (get_token(data)) {
            if (data->token->token != TYPE_KEYWORD || data->token->attr.keyword != KEYWORD_THEN) {
                return(ER_SYN);
            }
        } else
            return(ER_LEX);

        // ... EOL ...
        if (get_token(data)) {
            if (data->token->token != TYPE_EOL) {
                return(ER_SYN);
            }
        } else 
            return(ER_LEX);

        // ... <statements> ...
        if (get_token(data)) {
            if (!statement(data))
                return(ER_SYN);
        } else 
            return(ER_LEX);

        // ... ELSE ... || ... END ... - rozsirenie - volitelna cast ELSE
        if (get_token(data)) {
            // ELSE
            if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_ELSE) {
                // ... EOL ...
                if (get_token(data)) {
                    if (data->token->token != TYPE_EOL) {
                        return(ER_SYN);
                    }
                } else 
                    return(ER_LEX);
                
                // ... <statements> ...
                if (get_token(data)) {
                    if (!statement(data))
                        return(ER_SYN);
                } else 
                    return(ER_LEX);
                
                if (!get_token(data))
                    return(ER_LEX);
            }
            // END
            if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_END) {
                // ... EOL ...
                if (get_token(data)) {
                    if (data->token->token != TYPE_EOL) {
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
    // <statement> -> PRINT ( <argvs> ) EOL
    // <statement> -> LENGTH ( <argvs> ) EOL
    // <statement> -> SUBSTR ( <argvs> ) EOL
    // <statement> -> ORD ( <argvs> ) EOL
    // <statement> -> CHR ( <argvs> ) EOL
    // <statement> -> INPUTS EOL
    // <statement> -> INPUTI EOL
    // <statement> -> INPUTF EOL
    // <statement> -> ID <declare> EOL
}

static int declare(struct Data_t* data) {

}

static int params(struct Data_t* data) {

}

static int param(struct Data_t* data) {

}

static int argvs(struct Data_t* data) {

}

static int arg(struct Data_t* data) {

}

static int value(struct Data_t* data) {

}



static bool init_struct(struct Data_t* data){
    
    data->in_function = false;
    data->in_while_or_if = false;


    return true;
}

int start_parser(){

    struct Data_t* Dataptr = malloc(sizeof(struct Data_t));

    Token_t* one_token = Dataptr->token;
    one_token = malloc(sizeof(Token_t));
    struct string_t content;
    one_token->attr.string = &content;
    allocate_string(one_token->attr.string);

    int lex_result = get_next_token(one_token);
    if(lex_result == 0){
        if(init_struct(Dataptr)){
                prog(Dataptr);
            }
        }
    else{
        return parser_error(one_token, Dataptr);
        // parse error
    }

    free_string(one_token->attr.string);
    free(one_token);
    free(Dataptr);
    return 0;
}







