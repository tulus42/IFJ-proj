/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/


typedef struct Data_element{
    Data_t data;
    struct Data_element* next;
} DElem;

typedef struct Data_list{
    DElem* first;
    DElem* last;
} DList;

