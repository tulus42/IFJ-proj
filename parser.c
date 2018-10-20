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

void start_parser(){

    tDataptr list_of_tokens = malloc(sizeof(struct tData));
    list_of_tokens->token = malloc(sizeof(Token_t));
    list_of_tokens->token->token = TYPE_EOL;
    

    struct string_t identif_name;
    struct string_t *identif_ptr = &identif_name;

    allocate_string(identif_ptr);
    
    while(list_of_tokens->token->token != TYPE_EOF){
        int lex_result = get_next_token(list_of_tokens->token, identif_ptr);

        if(lex_result == 0){

            // debug prints from lexer
            if(list_of_tokens->token->token == TYPE_KEYWORD){
		        printf("%s : %s\n", tokens[list_of_tokens->token->token], keyword[list_of_tokens->token->attr.keyword]);
	        }
            else if(list_of_tokens->token->token == TYPE_IDENTIFIER){
                printf("%s : ", tokens[list_of_tokens->token->token]);
                printf("%s\n", identif_ptr->s);
            }
	        else if(list_of_tokens->token->token == TYPE_INT){
		        printf("%s : %d\n", tokens[list_of_tokens->token->token], list_of_tokens->token->attr.integer);
	        }
            else if(list_of_tokens->token->token == TYPE_FLOAT){
		        printf("%s : %f\n", tokens[list_of_tokens->token->token], list_of_tokens->token->attr.flt);
	        }
            else{
                printf("%s\n", tokens[list_of_tokens->token->token]);
            }
            clear_string_content(identif_ptr);
        }
        else{
            ; // lexer error
        }
    }
    

    free_string(identif_ptr);
    
    free(list_of_tokens->token);
    free(list_of_tokens);

}



