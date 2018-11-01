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
typedef struct {
	char *s;
	int current_size;
	int buffer_size;
} string_t;


//bool check_empty_bites(string_t *string_ptr);
//bool allocate_string(string_t *string_ptr);
//void free_string(string_t *string_ptr);
//bool add_char(string_t *string_ptr, char to_add);
//bool compare_strings(string_t *string_ptr, char word[]);
//bool check_comment_end(int match_count, string_t* string_ptr);
//bool check_comment_begin(int match_count, string_t* string_ptr);
//void clear_string_content(struct string_t* string_ptr);
//char convert_from_hex(char hex[]);
