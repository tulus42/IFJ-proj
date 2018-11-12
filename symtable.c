/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

// VARIANTA HASH TABLE

/*na konci vymaz htPrintTable a testing.h
  */


#include "sym_table.h"

//tStackP s;
tHTable local_ST;
tHTable global_ST;

#include "testing.h"

void htPrintTable( tHTable ptrht ) {
	//int maxlen = 0;
	//int sumcnt = 0;
	
	printf ("------------HASH TABLE--------------\n");
	for ( int i=0; i<HTSIZE; i++ ) {
		if ((ptrht)[i]==NULL)
		{
			continue;
		}
		printf ("%i:",i);
		//int cnt = 0;
		tHTItem* ptr = (ptrht)[i];
		while ( ptr != NULL ) {
			printf (" (%s,%s)",ptr->key->s,Types_of_tHTItem[ptr->typ]);
			//if ( ptr->ptrnext != NULL )
			//	cnt++;
			ptr = ptr->ptrnext;
		}
		//if (cnt > maxlen)
		//	maxlen = cnt;
		//sumcnt+=cnt;
	}
	printf ("koniec tabulky\n");
}

/**
 * Writes error to stderr, returns error type
 */
int sym_table_error(int error_code){
	htClearAll(local_ST);
	htClearAll(global_ST);
	fprintf(stderr, "sym_table ERROR\n");
return error_code;
}
/*
int sym_table_error(tHTable s, int error_type){
	free_string(s);
	fprintf(stderr, "sym_table ERROR\n");
return error_type;*/


/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( string_t* key ) {
	int retval = 1;
	int keylen = strlen(key->s);
	for ( int i=0; i<keylen; i++ )
		retval += key->s[i];
	//printf("%d\n",retval % HTSIZE );
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable ptrht ) {
	printf("som v htInit\n" );
	for (int i=0;i<HTSIZE;i++){		//cyklus nastavý kazdy prvok tabulky na NULL
			(ptrht)[i]=NULL;
	}
	printf("tabulka inicializovana\n" );
	//htPrintTable(ptrht);
return;
}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key->  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není, 
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable ptrht, string_t* key ) {
	printf("som v HTSearch\n");
	printf("som hladany kluc: %s\n", key->s );
	//printf("%d\n", hashCode((key)));
	tHTItem* tmp=(ptrht)[hashCode(key)]; 	//zahashuje kluc
	printf("%d\n", hashCode(key));
	printf("hash ok\n");
	if(tmp!=NULL){
		printf("%s\n", (ptrht)[hashCode(key)]->key->s);

		printf("%s\n",tmp->key->s );
		printf("%s\n", Types_of_tHTItem[tmp->typ]);
	}

	else
	printf("%d\n", hashCode(key));
	printf("hash ok\n");
	//printf("%s\n",tmp->key->s );
	while (tmp){							//cyklus prehladáva zoznam na danom mieste v tabulke
		//printf("%s\n",tmp->key->s );
		if (compare_dynamic_strings((tmp->key), key)){		//ak je item najdeny tak ho funkcia vrati
			printf("nasiel som zhodu\n");
			printf("koniec HTSearch\n");

			return tmp;
		}
		if (tmp->ptrnext==NULL){			//ak v zozname uz nie je ziadny dalsi item tak funkcia vrati NULL
			printf("not found\n");
			printf("koniec HTSearch\n");
			return NULL;
		}
		else{
			tmp=tmp->ptrnext;				//inak sa presunie na nasledujuci pointer
		}
	}
return NULL;
}




int def_ID( tHTable ptrht,string_t* key ){

	if (htSearch(ptrht,key)!=NULL){						//ak je item najdeny je to chyba
		printf("key najdeny\n");
		
		return sym_table_error(ER_SEM_VARIABLE);
 		 
 	}
 	else{
 		printf("key nenajdeny\n");
 		tHTItem *Ninsert = malloc(sizeof(tHTItem));	//ak nie je najdeny naalokuje si pamat pre dany item
 		if (Ninsert==NULL){							//ak sa alokacia nepodarila tak funkcia skonci
 			return sym_table_error(ER_INTERNAL);
 		}
 		printf("Ninsert ok\n");
 		/*
 		Ninsert->key= malloc(sizeof(string_t));
 		if (Ninsert->key==NULL){
 			printf("chyba\n");
 			return sym_table_error(ER_INTERNAL);
 		}*/
 		string_t string;
 		Ninsert->key=&string;


 		printf("pred alokate string\n");
 		if(!allocate_string(Ninsert->key)){
 			printf("chyba\n");
 			return sym_table_error(ER_INTERNAL);
 		}
 		printf("alokacie ok\n");
		/*if(allocate_string(Ninsert->data.string)){
 			return sym_table_error(ER_INTERNAL);
 		}*/
 		copy_string_content((Ninsert->key),key);			//nahra sa kluc
 		printf("nahratie kluca ok\n");
 		Ninsert->typ=NILL;							
 		printf("typ ok\n");
 		printf("%d\n", hashCode(key));
 		Ninsert->ptrnext=(ptrht)[hashCode(key)];	//prvok zaradime na zaciatok zoznamu
 		(ptrht)[hashCode(key)]=Ninsert;
 		printf("hash ok\n");
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

int htInsert ( tHTable ptrht, tHTItem* item_ptr) {

	tHTItem *actual_item=htSearch(ptrht,item_ptr->key);
	if (actual_item==NULL){	//ak je item niejenajdeny

		tHTItem *Ninsert = malloc(sizeof(tHTItem));	//ak nie je najdeny naalokuje si pamat pre dany item
 		if (Ninsert==NULL){							//ak sa alokacia nepodarila tak funkcia skonci
 			return sym_table_error(ER_INTERNAL);
 		}
 		if(allocate_string((Ninsert->key))){
 			return sym_table_error(ER_INTERNAL);
 		}
 		/*if(allocate_string(Ninsert->data.string)){
 			return sym_table_error(ER_INTERNAL);
 		}*/

 		copy_string_content((Ninsert->key),(item_ptr->key));			//nahra sa kluc
 		Ninsert->typ= item_ptr->typ;							
 		switch(item_ptr->typ){
 			case STRING:
 				//clear_string_content(Ninsert->data.string);
 				//copy_string_content((Ninsert->data.string), (item_ptr->data.string));
 			 	break;
 			case INTEGER:
 				//Ninsert->data.integer=item_ptr->data.integer;
 				break;
 			case FLT:
 				//Ninsert->data.flt=item_ptr->data.flt;
 				break;
 			case FUNCTION:
 				Ninsert->param_count=item_ptr->param_count;
 				Ninsert->defined=item_ptr->defined;
 				break;		
 			default:
 				return  sym_table_error(ER_SEM_TYPE);
 				break;
 		}
 		Ninsert->ptrnext=(ptrht)[hashCode((item_ptr->key))];	//prvok zaradime na zaciatok zoznamu
 		(ptrht)[hashCode((item_ptr->key))]=Ninsert;
 		return 0;
	}else{	

		//ak je len jeden z itemov typu funkcia vracia sa error	
		if (((actual_item->typ==FUNCTION)&&(item_ptr->typ!=FUNCTION))||((item_ptr->typ==FUNCTION)&&(actual_item->typ!=FUNCTION))){
			return  sym_table_error(ER_SEM_TYPE);
		}//Ak su obe funkcie
		if ((actual_item->typ==FUNCTION)&&(item_ptr->typ==FUNCTION)){
			if ((!actual_item->defined)&&(item_ptr->defined)&&(actual_item->param_count==item_ptr->param_count)){		//ako je to s optional parametrami?
 				actual_item->defined=TRUE;
 				return 0;
 			}else{
 				return  sym_table_error(ER_SEM_TYPE);
 			}
		}else{
			switch(item_ptr->typ){
				case STRING:
					actual_item->typ= item_ptr->typ;
					//clear_string_content(actual_item->data.string);
	 				//copy_string_content(actual_item->data.string, item_ptr->data.string);
				 	break;
				case INTEGER:
					actual_item->typ= item_ptr->typ;
					//actual_item->data.integer=item_ptr->data.integer;
					break;
				case FLT:
					actual_item->typ= item_ptr->typ;
					//actual_item->data.flt=item_ptr->data.flt;
					break;
				default:
					return  sym_table_error(ER_SEM_TYPE);
 					break;
			}
 

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
/*
tHTItem* htRead (string_t key) {

	if (htSearch(local_ST,key)!=NULL){				//ak je item najdeny
		return (htSearch(local_ST,key));	//funkcia vrati data hladaneho itemu
	}
	else{										//inak vrati NULL
		return (htSearch(global_ST,key));
	}
}
*/

Type_of_tHTItem get_type (tHTable ptr,string_t* key) {

	printf("som v get_type\n");
	printf("%s\n",key->s );
	tHTItem *tmp=(htSearch(ptr,key));
	printf("%s\n",tmp->key->s);
									
	return (tmp->typ);
}


int check_define (string_t* key) {		//defined==TRUE==1 ; defined==FALSE==0; not_a_function==2

	tHTItem *tmp = htSearch(global_ST,key);

	if (tmp->typ!=FUNCTION){
		return 2;				
	}else{
		return (tmp->defined);
	}								
	
	
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable ptrht ) {



	tHTItem *tmp; //pomocny pointer
	for (int i=0;i<HTSIZE;i++){	//cyklus prechadza tabulkov
		
		tmp=(ptrht)[i];

		while((ptrht)[i]!=NULL){	//cyklus prechadza zoznamom
			//tmp=(*ptrht)[i];		//postupne uvolni cely zoznam
			//free_string(tmp->data.string);
			(ptrht)[i]=(ptrht)[i]->ptrnext;
			free(tmp);
		}
	}
	htInit(ptrht);					//reinicializacia tabulky
}


//kontroluje ci su vvsetky funkcie definovane
int STlast_check(){

	tHTItem *tmp; //pomocny pointer
	
	for (int i=0;i<HTSIZE;i++){	//cyklus prechadza tabulkov
		
		tmp=global_ST[i];

		while(tmp!=NULL){	//cyklus prechadza zoznamom
			if ((tmp->typ==FUNCTION)&&(tmp->defined==FALSE)){
				return sym_table_error(ER_SEM_VARIABLE);
			}
			tmp=tmp->ptrnext;
		}
	}
	return 0;
}

void STinits(){
	htInit(local_ST);
	htInit(global_ST);

}
