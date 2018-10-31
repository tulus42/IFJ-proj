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

// Frees all the memory
int parser_error(Token_t* token, struct Data_t* Dataptr){
    // frees the memory
    free_string(token->attr.string);
    free(token);
    free(Dataptr);

    return ER_SYN;
}

static int prog(struct Data_t* data){

    // <prog> -> DEF ID_FUNC ( <params> ) EOL <statement> END <prog>
    if(data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_DEF){

    } // <prog> -> EOL <prog>
    else if(data->token->token == TYPE_EOL){

    } // <prog> -> EOF
    else if(data->token->token == TYPE_EOF){

    } // <prog> -> <statements> <prog> ???? toto treba ešte domyslieť
    else if(data->token->token == TYPE_KEYWORD || data->token->token == TYPE_IDENTIFIER){

    }
    else{ // nothing good was entered
        return ER_SYN;
    }

    return 0;
}

static bool init_struct(struct Data_t* data){
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
        // parse
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







