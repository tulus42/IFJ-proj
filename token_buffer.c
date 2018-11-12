/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

#include <stdio.h>
#include "token_buffer.h"

void ListInit(DList* L){
    L->last = NULL;
    L->first = NULL;
}

void DisposeList(DList* L){
    L->last = NULL;
    while(L->first != NULL){
        DElem* next_element = L->first->next;
        DElem* to_delete = L->first;

        free(to_delete);
        L->first = next_element;
    }
}