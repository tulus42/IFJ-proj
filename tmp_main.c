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
    get_source(input_file);

    start_parser();
}