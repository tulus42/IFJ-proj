/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#include "error.h"
#include "lexer.h"

FILE* source;

void get_source(FILE *f)
{
	source = f;
}

int get_next_token() // konecny automat, v podstate while cyklus, ve kterem je switch, nacitame znaky, jakmile urcime token tak ho vratime nebo najdeme blbost a vratime ER_LEX
{
	int current_status = STATE_START;
	if (source == NULL)
	{
		return ER_INTERNAL;
	}
	
	while(1){
		char c = getc(source); // read characters one by one
		
		switch(current_status){
			case(STATE_START): // first comparison
			if(c == '*'){
				;
			}
			else if(c == '+'){
				;
			}
			else if(c == '/'){
				;
			}
			else if(c == '-'){
				;
			}
			else if(c == '<'){
				;
			}
			else if(c == '>'){
				;
			}
			else if(c == '#'){
				;
			}
			else if(c == '('){
				;
			}
			else if(c == ')'){
				;
			}
			else if(c == ','){
				;
			}
			else if(isdigit(c)){
				;
			}
			else if(ischar(c)){
				;
			}
			break;
		}
		if(c == EOF){
			break;
		}
	}
	return 0;
}