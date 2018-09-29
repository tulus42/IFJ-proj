/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/


#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "instructions.h"



int main(int argc, char* argv[])
{
	FILE* source;

	source = stdin;


	get_source(source);

	int error = 0;
/*	if (error = parse())
	{
		return error;
	} 
*/

	return EXIT_SUCCESS;

}