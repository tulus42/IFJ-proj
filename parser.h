/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

#include <stdlib.h>
#include <stdbool.h>

#include "scanner.h"

#ifndef PARSER
#define PARSER

#define SYN_OK 0

typedef struct {
    Token_t* token;

    
    bool in_bracket;
    int in_while_or_if;
    bool in_definition;
    bool in_declare;

} Data_t;

// forward declarations

int statement(Data_t* data);
int declare(Data_t* data);
int params(Data_t* data);
int param(Data_t* data);
int argvs(Data_t* data);
int arg(Data_t* data);
int function(Data_t* data);
int print(Data_t* data);


int start_parser();
int buffer_testing(Data_t* data);
int parser_error(Data_t* Data, string_t* string);


#endif