/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

#include <stdio.h>

#include "parser.h"
#include "error.h"


int main(){
    FILE* input_file = stdin;
    int parser_result;
    get_source(input_file);

    ;

    generator_start(); // kontrolovat succes

    parser_result = start_parser();
    if(parser_result == ER_SYN){
        return 1;
    }
    
    flush_code(stdout);

    return 0;
}