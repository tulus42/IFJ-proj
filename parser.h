/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

#include <stdbool.h>

#define SYN_OK 0

struct Data_t{
    Token_t* token;

    
    bool in_function;
    bool in_while_or_if;

} *Dataptr;