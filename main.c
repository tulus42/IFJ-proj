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
#include "instructions.h"


int main(){
    FILE* input_file = stdin;
    int parser_result;
    get_source(input_file);


    if (generator_start() == false) { 
        return(99);
    }

    parser_result = start_parser();
    
    
    if(parser_result == SYN_OK){
        
        flush_code(stdout);

    }else
    {
        clear_code();
        return(parser_result);
    }
   

    return 0;
}
