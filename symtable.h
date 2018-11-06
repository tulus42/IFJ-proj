
/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/


#ifndef _HASHTABLE_H_
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


/* typ klíče (například identifikace zboží) */
typedef char tKey;

typedef  Type_of_tHTItem ttyp;

/* typ obsahu (například cena zboží) */
typedef union tData;

/*Datová položka TRP s explicitně řetězenými synonymy*/
 typedef struct tHTItem{
	tKey key[];				/* klíč  */
  ttype typ;    //najdlhí typ function má 8 znakov
	tData data;				/* obsah */
	struct tHTItem* ptrnext;	/* ukazatel na další synonymum */
  bool defined; 
} tHTItem;

typedef struct {                          /* zásobník hodnot typu thtable */
    tHTable* a;
    int top;
    int count;
} tStackP;

/* TRP s explicitně zřetězenými synonymy. */
typedef tHTItem* tHTable[HTSIZE];

/* Pro účely testování je vhodné mít možnost volby velikosti pole,
   kterým je vyhledávací tabulka implementována. Fyzicky je deklarováno
   pole o rozměru HTSIZE, ale při implementaci vašich procedur uvažujte
   velikost HTSIZE.  Ve skriptu se před voláním řešených procedur musí
   objevit příkaz HTSIZE N, kde N je velikost požadovaného prostoru.
   
   POZOR! Pro správnou funkci TRP musí být hodnota této proměnné prvočíslem.
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
