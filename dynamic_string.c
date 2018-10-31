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


/**
 * Allocates string with starting size of 10 
 */
bool allocate_string(struct string_t* string_ptr){
    string_ptr->s = (char *) malloc(STARTING_SIZE);
    if(string_ptr->s == NULL){
        fprintf(stderr, "Failed to reallocate memory\n");
        return false;
    }
    else{
        string_ptr->buffer_size = STARTING_SIZE;
        string_ptr->current_size = 0;
        string_ptr->s[string_ptr->current_size] = '\0';
        return true;
    }
}

/**
 * Frees string 
 */
void free_string(struct string_t* string_ptr){
    free(string_ptr->s);
}

/**
 * Check whether there is still enough space, if not, reallocate and add 5 bites
 */
bool check_empty_bites(struct string_t *string_ptr){
    if(string_ptr->current_size == (string_ptr->buffer_size - 2)){
        string_ptr->s = realloc(string_ptr->s, string_ptr->buffer_size + NEW_ALLOCATION);
        if(string_ptr->s == NULL){
            fprintf(stderr, "Failed to reallocate memory\n");
            free_string(string_ptr);
            return false; /// PATRI TOTO SEM??????
        }
        else{
            string_ptr->buffer_size += NEW_ALLOCATION;
            return true; 
        }
    }
    else{
        return true;
    }
}

/**
 * Adds char to first empty space in string
 */
bool add_char(struct string_t* string_ptr, char to_add){
    if(!check_empty_bites(string_ptr)){
        return false;
    }
    else{
        string_ptr->s[string_ptr->current_size] = to_add;
        string_ptr->current_size++;
        string_ptr->s[string_ptr->current_size] = '\0';
        return true;
    }
}

/**
 * Compares each char one by one and decides, whether it is a keyword or not
 */
bool compare_strings(struct string_t* string_ptr, char word[]){
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

/**
 * Compares input with '=begin'
 */
bool check_comment_begin(int match_count, struct string_t* string_ptr){
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

/**
 * Compares input with '=end'
 */
bool check_comment_end(int match_count, struct string_t* string_ptr){
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

/**
 * Clears string content and inserts '\0'
 * Size is set to 0
 */
void clear_string_content(struct string_t* string_ptr){
    for(int i = 0; i < string_ptr->current_size; i++){
        string_ptr->s[i] = '\0';
    }
    string_ptr->current_size = 0;
}

/**
 * Converts hexadecinal number to decimal number
 */
char convert_from_hex(char hex[]){
    long n = strtol(hex, NULL, 16);
    int tmp = (int) n;
    return tmp;
}

void remove_first_char(struct string_t* string_ptr){
    char one_char;
    int counter = 1;
    while(string_ptr->s[counter] != '\0'){
        one_char = string_ptr->s[counter];
        string_ptr->s[counter-1] = one_char;
        string_ptr->current_size = counter;
        counter++;
    }
    string_ptr->s[counter-1] = '\0';
}

void copy_string_content(struct string_t* dest, struct string_t* source){
    for(int i = 0; i < source->current_size; i++){
        add_char(dest, source->s[i]);
    }
}