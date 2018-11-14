/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

#include <stdbool.h>

#ifndef DYNAMIC_STRING 
#define DYNAMIC_STRING

// size for allocations
#define STARTING_SIZE 10
#define NEW_ALLOCATION 5

// struct for dynamic string
typedef struct {
	char *s;
	unsigned int current_size;
	unsigned int buffer_size;
} string_t;

bool add_char(string_t* string_ptr, char to_add);
bool allocate_string(string_t* string_ptr);
bool check_empty_bites(string_t *string_ptr);
bool compare_strings(string_t* string_ptr, char word[]);
bool compare_dynamic_strings(string_t* string_ptr, string_t* another_ptr);
bool check_comment_begin(int match_count, string_t* string_ptr);
bool check_comment_end(int match_count, string_t* string_ptr);
bool add_string(string_t* string_ptr, char to_add[]);

void free_string(string_t* string_ptr);
void clear_string_content(string_t* string_ptr);
void remove_first_char(string_t* string_ptr);
void copy_string_content(string_t* dest, string_t* source);

char convert_from_hex(char hex[]);

#endif
