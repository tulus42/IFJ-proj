/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

#include "parser.c"

int main(){
    FILE* input_file = stdin;
    int parser_result;
    get_source(input_file);

    parser_result = start_parser();
    printf("Main ended with exit code: %d\n", parser_result);
    if(parser_result == ER_SYN){
        return 1;
    }
    return 0;
}