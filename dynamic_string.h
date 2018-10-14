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
struct string_t {
	char *s;
	int current_size;
	int buffer_size;
};



//void init_struc_pointer(struct string_t **string_ptr);
//void free_struc_pointer(struct string_t **string_ptr);
//void check_empty_bites(struct string_t *string_ptr);
//void allocate_string(struct string_t *string_ptr);
//void free_string(struct string_t *string_ptr);
//void add_char(struct string_t *string_ptr, char to_add);
//bool compare_strings(struct string_t *string_ptr, char word[]);
