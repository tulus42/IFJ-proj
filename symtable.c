/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

// VARIANTA HASH TABLE

/*TODO UPDATE_ITEM()
  TODO GIVE_TYPE()
  TODO SEARCHLOCGLOBAL()
  */


#include "sym_table.h"


/**
 * Writes error to stderr, returns error type
 */
int sym_table_error(tHTable* stack_a, int error_type){
	if (stack_a->top!=0){
		for (int i = 0; i < stack_a->top; i++){
			htClearAll(stack_a->a[i]);
		}
	}
	free_string(stack_a);
	fprintf(stderr, "sym_table ERROR\n");
return error_type;
}
/*
int sym_table_error(tHTable* stack_a, int error_type){
	free_string(stack_a);
	fprintf(stderr, "sym_table ERROR\n");
return error_type;*/


/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht ) {

	for (int i=0;i<HTSIZE;i++){		//cyklus nastavý kazdy prvok tabulky na NULL
			(*ptrht)[i]=NULL;
		}
return;
}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není, 
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {

	tHTItem *tmp=(*ptrht)[hashCode(key)]; 	//zahashuje kluc

	while (tmp){							//cyklus prehladáva zoznam na danom mieste v tabulke
	
		if (strcmp(tmp->key, key)==0){		//ak je item najdeny tak ho funkcia vrati
			return tmp;
		}
		if (tmp->ptrnext==NULL){			//ak v zozname uz nie je ziadny dalsi item tak funkcia vrati NULL
			return NULL;
		}
		else{
			tmp=tmp->ptrnext;				//inak sa presunie na nasledujuci pointer
		}
	}
return NULL;
}




int def_ID( tHTable* ptrht,tKey key ){

	if (htSearch(ptrht,key)){						//ak je item najdeny je to chyba
		
		return sym_table_error(s->a,ER_SEM_VARIABLE);
 		 
 	}
 	else{
 		tHTItem *Ninsert = malloc(sizeof(tHTItem));	//ak nie je najdeny naalokuje si pamat pre dany item
 		if (Ninsert==NULL){							//ak sa alokacia nepodarila tak funkcia skonci
 			return;
 		}

 		Ninsert->key=key;							//nahra sa kluc
 		Ninsert->typ=NILL;							//TO DO moze sa to takto priradzovat?
 		Ninsert->data.string="nil";							//nahraju sa data
 		Ninsert->ptrnext=(*ptrht)[hashCode(key)];	//prvok zaradime na zaciatok zoznamu
 		(*ptrht)[hashCode(key)]=Ninsert;
 	}
	return 0;
}

/* 
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

int htInsert ( tHTable* ptrht, tHTItem* item_ptr) {

	tHTItem *actual_item=htSearch(ptrht,key);
	if (actual_item==NULL){	//ak je item niejenajdeny

		tHTItem *Ninsert = malloc(sizeof(tHTItem));	//ak nie je najdeny naalokuje si pamat pre dany item
 		if (Ninsert==NULL){							//ak sa alokacia nepodarila tak funkcia skonci
 			return sym_table_error(s->a,ER_INTERNAL);
 		}
 		Ninsert->key= item_ptr->key;							//nahra sa kluc
 		Ninsert->typ= item_ptr->typ;							
 		swich(item_ptr->typ){
 			case STRING:					//TO DO moze sa to takto priradzovat?
 			 	Ninsert->data.string=item_ptr->data.string;
 			 	break;
 			case INTEGER:
 				Ninsert->data.integer=item_ptr->data.integer;
 				break;
 			case FLT:
 				Ninsert->data.flt=item_ptr->data.flt;
 				break;
 			case FUNCTION:
 				Ninsert->data.integer=item_ptr->data.integer;
 				Ninsert->data.defined=item_ptr->data.defined;
 				break;	
 			default:
 				//TO DO ER_WRONG_OPERAND_TYPE
 				break;
 		}
 		Ninsert->ptrnext=(*ptrht)[hashCode(key)];	//prvok zaradime na zaciatok zoznamu
 		(*ptrht)[hashCode(key)]=Ninsert;
	}else{											//update
 		if (actual_item->typ==item_ptr->typ){
	  		swich(item_ptr->typ){					
	 			case STRING:					
	 			 	Ninsert->data.string=item_ptr->data.string;
	 			 	break;
	 			case INTEGER:
	 				Ninsert->data.integer=item_ptr->data.integer;
	 				break;
	 			case FLT:
	 				Ninsert->data.flt=item_ptr->data.flt;
	 				break;
	 			case NILL:
 					Ninsert->data=item_ptr->data;
 					break;
	 			default:
	 				return  sym_table_error(s->a,ER_SEM_TYPE);
	 				break;			
 			}
 		}else{
 			return  sym_table_error(s->a,ER_SEM_TYPE);

 		}
 	}
 return 0;
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

//TO DO ako vratit hodnotu
//READ je treba???

tHTItem htRead ( tHTable* ptrht, tKey key  ) {

	if (htSearch(ptrht,key)!=NULL){				//ak je item najdeny
		return &(htSearch(ptrht,key))->data;	//funkcia vrati data hladaneho itemu
	}
	else{										//inak vrati NULL
		return NULL;
	}
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {

 	tHTItem *tmp=(*ptrht)[hashCode(key)];		//tmp ukazuje tam kam ukazuje prvy prvok zoznamu na danom mieste v tabulke

	if (strcmp(tmp->key, key)==0){				//ak je prvy prvok zoznamu rovny hladanemu prvku
 		(*ptrht)[hashCode(key)]= tmp->ptrnext;	//prvy prvok bude ukazovať na nasledujuci prvok
 		free(tmp);								//uvolni sa pozadovany prvok
 		return;
	}
	tHTItem *to_delete;							//vytvori sa pomocny pointer

	while (tmp){								//prechadza prvky zoznamu
		
		if (strcmp(tmp->ptrnext->key, key)==0){	//ak je prvok najdeny
			to_delete=tmp->ptrnext;				//pomocny pointer ukazuje na hladany item
			tmp->ptrnext=to_delete->ptrnext;	//zoznam sa previaze
			free(to_delete);					//uvolni sa pamat hladaneho itemu
			return;
		}
		else{
			if (tmp->ptrnext==NULL){			//ak hladany prvok nebol najdeny nestane sa nic
				return;
			}
			tmp=tmp->ptrnext;					//posun v zozname
		}
	}
return;
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht ) {



	tHTItem *tmp; //pomocny pointer
	for (int i=0;i<HTSIZE;i++){	//cyklus prechadza tabulkov
		
		tmp=(*ptrht)[i];

		while((*ptrht)[i]!=NULL){	//cyklus prechadza zoznamom
			tmp=(*ptrht)[i];		//postupne uvolni cely zoznam
			(*ptrht)[i]=(*ptrht)[i]->ptrnext;
			free(tmp);
		}
	}
	htInit(ptrht);					//reinicializacia tabulky
}

//Stack ops

int FrameStackInit ()  
/*   ------
** Inicializace zásobníku.
**/
{
	S->top = 0;
	s->count=STACK_SIZE;  
	s->a = (tHTable*) malloc(s->count*sizeof(tHTable));
	if (s->a==NULL){
		return sym_table_error(s->a,ER_INTERNAL);
	}
	//Global Frame init
	tHTable* GlobalFrame;
	htInit(GlobalFrame);
	PushFrame(S,GlobalFrame);
	
	return 0;
}	

int PushFrame (tHTable* ptr)
/*   ------
** Vloží hodnotu na vrchol zásobníku.
**/
{ 
	/* Při implementaci v poli může dojít k přetečení zásobníku. */
  	if (S->top==s->count){
    	s->count=(s->count)*2 
      	s->a=realloc(s->a,s->count*sizeof(tHTable))
      	if (s->a==NULL){
  			return sym_table_error(s->a,ER_INTERNAL);
  		}
  	}else {  
		S->top++;  
		S->a[S->top]=ptr;
	}
	return 0;
}	

int PopFrame (tHTable* ptr )
/*         --------
** Odstraní prvek z vrcholu zásobníku a současně vrátí jeho hodnotu.
**/
{
  /* Operace nad prázdným zásobníkem způsobí chybu. */
	if (S->top==0)  {
		return sym_table_error(s->a,ER_FRAME);	
	}	
	else {
		htClearAll(S->a[S->top])
		ptr=(S->a[S->top--]);
		return 0;
	}	
}

bool EmptyStack ()
/*   -------
** Je-li zásobník prázdný, vrátí hodnotu true.
**/
{
  return(S->top==0);
}

//dealokuje pole a nastavy vrchol a pocitadlo velkosti na 0
int DeleteStack(){
	if (stack_a->top!=0){
		for (int i = 0; i < stack_a->top; i++){
			htClearAll(stack_a->a[i]);
		}
	}
	S->top = 0;
	s->count=0;  
	free(s->a);
	return 0;
}
