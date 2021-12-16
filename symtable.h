/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "error.h"


#define HTSIZE 6421
#define FUNCTION_DEFINED 1
#define FUNCTION_UNDEFINED 0
#define ST_OK 0
#define NOT_FOUND 2
#define PARAM_DEFINED 3
#define PARAM_UNDEFINED 4

#ifndef SYMTABLE_H
#define SYMTABLE_H



/*structers*/

typedef enum
{
  NILL,
  STRING,
  INTEGER,
  PRASATKO_S_PAPUCKAMI_FLT,
  VAR,
  FUNCTION,

} Type_of_tHTItem;


 typedef struct tHTItem{
  char* key;       
  Type_of_tHTItem typ;    
  bool defined;
  int param_count;
  struct tHTItem* ptrnext;
} tHTItem;

typedef tHTItem* tHTable[HTSIZE];

tHTable local_ST;
tHTable global_ST;




/* headers */

int sym_table_error(int error_code);
int hashCode ( char key[] );
void htInit ( tHTable ptrht );
void STinits();
tHTItem* htSearch ( tHTable ptrht, char key[] );
int def_ID( tHTable ptrht,char key[] );
int htInsert ( tHTable ptrht, tHTItem* item_ptr );
Type_of_tHTItem* get_type (tHTable ptrht,char key[]);
int check_define (tHTable tab, char key[]);
void htClearAll ( tHTable ptrht );
int STlast_check();
void htClearAlltables();
void htClearlocal ();
int iteminit(tHTItem* item,char k[], Type_of_tHTItem t,bool d, int pc );
void itemfree(tHTItem* item);
int itemupdate(tHTItem* item,char k[], Type_of_tHTItem t,bool d, int pc );
void htPrintTable( tHTable ptrht );
int check_param_cnt(char key[]);

#endif
