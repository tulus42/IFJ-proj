/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

// size for allocations
#define STARTING_SIZE 10
#define NEW_ALLOCATION 5

// struct for dynamic string
typedef struct string
{
	char *s;
	int size;
	int b_size;
} string;


// TODO
// add_char
// reallocate
// allocate string size
// free string