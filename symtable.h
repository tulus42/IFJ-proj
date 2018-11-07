
/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/


#define _HASHTABLE_H_

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

typedef enum
{
  NILL,
  STRING,
  INTEGER,
  FLT,
  FUNCTION,

} Type_of_tHTItem;


/* typ klíče (například identifikace zboží) */
typedef char tKey;

typedef  Type_of_tHTItem ttyp;

/* typ obsahu (například cena zboží) */
typedef union tData;

typedef struct tHvar{
  tData data;       /* obsah */
}tHvar;

typedef struct tHfunction{
  bool defined;
  int param_count;
}tHfunction;

/*Datová položka TRP s explicitně řetězenými synonymy*/
 typedef struct tHTItem{
  tKey key[];       /* klíč  */
  ttyp typ;    //najdlhí typ function má 8 znako
  tHfunction function;
  tHvar var;
  struct tHTItem* ptrnext;
} tHTItem;

typedef struct {                          /* zásobník hodnot typu thtable */
    tHTable* a;
    int top;
    int count;
} tStackP;

/* TRP s explicitně zřetězenými synonymy. */
typedef tHTItem* tHTable[HTSIZE];

