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
    string_ptr->s[string_ptr->current_size] = to_add;
    string_ptr->current_size++;
    string_ptr->s[string_ptr->current_size] = '\0';
}

// true if they are the same, false if not the same
bool compare_strings(struct string_t *string_ptr, char word[]){
    bool tmp_result;
    if(string_ptr->current_size == strlen(word)){
        for(int i = 0; i < strlen(word); i++){
            if(string_ptr->s[i] == word[i]){
                tmp_result = true;
            }
            else{
                tmp_result = false;
                break;
            }
        }
        return tmp_result;
    }
    else{
        return false;
    }
}

bool check_comment_begin(int match_count, struct string_t *string_ptr){
    char* expected_input = "=begin";
    for(int i = 0; i < match_count; i++){
        if(string_ptr->s[i] == expected_input[i])
            continue;
        else{
            return false;
        }
    }
    return true;
}

bool check_comment_end(int match_count, struct string_t *string_ptr){
    char* expected_input = "=end";
    for(int i = 0; i < match_count; i++){
        if(string_ptr->s[i] == expected_input[i])
            continue;
        else{
            return false;
        }
    }
    return true;
}