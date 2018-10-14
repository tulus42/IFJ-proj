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

void allocate_string(struct string_t *string_ptr){
    string_ptr->s = (char *) malloc(STARTING_SIZE);
    if(string_ptr->s == NULL){
        printf("Failed to reallocate memory\n");
        exit(1); /// PATRI TOTO SEM???????????
    }
    else{
        string_ptr->buffer_size = STARTING_SIZE;
        string_ptr->current_size = 0;
        string_ptr->s[string_ptr->current_size] = '\0';
    }
}

void free_string(struct string_t *string_ptr){
    free(string_ptr->s);
}

void check_empty_bites(struct string_t *string_ptr){
    if(string_ptr->current_size == (string_ptr->buffer_size - 2)){
        string_ptr->s = realloc(string_ptr->s, NEW_ALLOCATION);
        if(string_ptr->s == NULL){
            printf("Failed to reallocate memory\n");
            free_string(string_ptr);
            exit(1); /// PATRI TOTO SEM??????
        }
        else{
            string_ptr->buffer_size += NEW_ALLOCATION; 
        }
    }
    else{
        return;
    }
}

void add_char(struct string_t *string_ptr, char to_add){
    check_empty_bites(string_ptr);
    // check whether it is empty
    //check_empty_bites(&string_ptr);
    printf("Size je: %d\n", string_ptr->current_size);
    printf("Buffer size is %d\n", string_ptr->buffer_size);
    printf("Char to add is %c\n", to_add);
    string_ptr->s[string_ptr->current_size] = to_add;
    string_ptr->current_size++;
    string_ptr->s[string_ptr->current_size] = '\0';
    printf("The string is %s\n\n", string_ptr->s);
}

void free_dynamic_string(struct string_t *string_ptr){
    ;
}