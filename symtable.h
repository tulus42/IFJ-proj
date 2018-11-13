
/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "error.h"


/* Maximální velikost pole pro implementaci
   vyhledávací tabulky. Řešené procedury však
   využívají pouze HTSIZE prvků pole (viz deklarace této proměnné).
*/
#define HTSIZE 6421
#define STACK_SIZE 30
#define TRUE 1
#define FALSE 0
typedef enum
{
  NILL,
  STRING,
  INTEGER,
  FLT,
  FUNCTION,

} Type_of_tHTItem;

/*
typedef struct tHfunction{
   bool defined;
  int param_count;

}tHfunction;
*/

/*Datová položka TRP s explicitně řetězenými synonymy*/
 typedef struct tHTItem{
  char* key;       
  Type_of_tHTItem typ;    
  bool defined;
  int param_count;
  //Token_attr data;
  struct tHTItem* ptrnext;
} tHTItem;

/* TRP s explicitně zřetězenými synonymy. */
typedef tHTItem* tHTable[HTSIZE];
/*
typedef struct {                          
    tHTable* a;
    int top;
    int count;
} tStackP;
*/

//extern int HTSIZE;

/* Hlavičky řešených procedur a funkcí. */
/*
int hashCode ( tKey key );

void htInit ( tHTable* ptrht );

tHTItem* htSearch ( tHTable* ptrht, tKey key );

void htInsert ( tHTable* ptrht, tKey key, tData data );

tData* htRead ( tHTable* ptrht, tKey key );

void htDelete ( tHTable* ptrht, tKey key );

void htClearAll ( tHTable* ptrht );

#endif
*/
