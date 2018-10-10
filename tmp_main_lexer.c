/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

#include "lexer.c"

int main(){
    Token_t* z;
    FILE* input_file = stdin;
    get_source(input_file);

    z = malloc(sizeof(Token_t));
    get_next_token(z);
    free(z);
}