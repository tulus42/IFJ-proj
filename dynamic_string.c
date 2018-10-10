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

#include "dynamic_string.h"

void init_struc_pointer(string* string_ptr){
    string_ptr = malloc(sizeof(struct string));
}

void free_struc_pointer(string* string_ptr){
    free(string_ptr->s);
    string_ptr->size = 0;
    string_ptr->b_size = 0;
    free(string_ptr);
}

// TODO - KONCIM? IDEM DALEJ? CO MAM ROBIT
void check_empty_bites(string* string_ptr){
    if(string_ptr->size == (string_ptr->b_size - 2)){
        string_ptr->s = realloc(string_ptr->s, NEW_ALLOCATION*sizeof(char));
        if(string_ptr->s == NULL){
            fprintf(stderr, "Failed to reallocate memory\n");
            free_string(string_ptr);
            free_struc_pointer(string_ptr);
            exit(1); /// PATRI TOTO SEM??????
        }
        else{
            string_ptr->b_size += NEW_ALLOCATION; 
        }
    }
    else{
        return;
    }
}

void allocate_string(string* string_ptr){
    string_ptr->s = malloc(STARTING_SIZE*sizeof(char));
    if(string_ptr->s == NULL){
        fprintf(stderr, "Failed to reallocate memory\n");
        free_struc_pointer(string_ptr);
        exit(1); /// PATRI TOTO SEM???????????
    }
    else{
        string_ptr->b_size = STARTING_SIZE;
    }
}

void free_string(string* string_ptr){
    string_ptr->b_size = 0;
    string_ptr->size = 0;
    free(string_ptr);
}

void add_char(string* string_ptr, char to_add){
    // check whether it is empty
    check_empty_bites(string_ptr);
    string_ptr->s[string_ptr->size] = to_add;
    string_ptr++;

}