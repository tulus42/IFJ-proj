/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

#include <stdbool.h>

#define SYN_OK 0

typedef struct {
    Token_t* token;

    
    bool in_bracket;
    int in_while_or_if;
    bool in_definition;

} Data_t;