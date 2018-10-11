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



string* init_struc_pointer(string* string_ptr);
void free_struc_pointer(string* string_ptr);
void check_empty_bites(string* string_ptr);
string* allocate_string(string* string_ptr);
void free_string(string* string_ptr);
void add_char(string* string_ptr, char to_add);
bool compare_strings(string* string_ptr, char word[]);
