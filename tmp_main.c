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
    
    parser_result = start_parser();
    if(parser_result == ER_SYN){
        return 1;
    }
    
    return 0;
}