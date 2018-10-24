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
#include "parser.h"
#include "instructions.h"

#include "lexer.c"
#include "parser.h"

/**
 * TODO: 
 * nový main
 * kontrola 0 alebo 1 LEX. analýzy
 * allocation of tokens
 * structures for parser.h
 * viazaný zoznam
 * procedenční tabuľka
 * LL-gramatika
 */

typedef struct tData{
    Token_t* token;
    struct tData *lptr;
    struct tData *rptr;
} *tDataptr;


// Frees all the memory
int parser_error(struct tData* list_of_tokens, struct string_t* identif_ptr){
    // frees the memory
    free_string(identif_ptr);
    free(list_of_tokens->token);
    free(list_of_tokens);

    return ER_SYN;
}

int start_parser(){

    tDataptr list_of_tokens = malloc(sizeof(struct tData));
    list_of_tokens->token = malloc(sizeof(Token_t));
    list_of_tokens->token->token = TYPE_EOL;
    

    struct string_t identif_name;
    struct string_t *identif_ptr = &identif_name;
    allocate_string(identif_ptr);
    

    int lex_result = get_next_token(list_of_tokens->token, identif_ptr);
    if(lex_result == 0){
        ; // Parsing
    }
    else{
        return parser_error(list_of_tokens);
        ; // ERROR
    }
    

    free_string(identif_ptr);
    free(list_of_tokens->token);
    free(list_of_tokens);

    return 0;
}





