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
			printf (" (%s,%s)\n",ptr->key,Types_of_tHTItem[ptr->typ]);
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
	//htInit(local_ST);
	htClearAll(global_ST);
	//htInit(global_ST);
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

int hashCode ( char key[] ) {
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

void htInit ( tHTable ptrht ) {

	for (int i=0;i<HTSIZE;i++){		//cyklus nastavý kazdy prvok tabulky na NULL
			(ptrht)[i]=NULL;
		}
return;
}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není, 
** vrací se hodnota NULL.
**
*/
void STinits(){
	htInit(global_ST);
	htInit(local_ST);
}

tHTItem* htSearch ( tHTable ptrht, char key[] ) {

	tHTItem *tmp=(ptrht)[hashCode(key)]; 	//zahashuje kluc

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


int def_ID( tHTable ptrht,char key[] ){

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
 		
		Ninsert->key = (char*) malloc((strlen(key)+2));
		if (Ninsert->key==NULL){							//ak sa alokacia nepodarila tak funkcia skonci
 			return sym_table_error(ER_INTERNAL);
 		}

 		strcpy(Ninsert->key,key);
 		printf("nahratie kluca ok\n");
 		Ninsert->typ=NILL;	
 		Ninsert->param_count=0;
 		Ninsert->defined=FALSE;						
 		printf("typ ok\n");
 		printf("som hash prvku : %d\n", hashCode(key));
 		Ninsert->ptrnext=(ptrht)[hashCode(key)];	//prvok zaradime na zaciatok zoznamu
 		(ptrht)[hashCode(key)]=Ninsert;
 		printf("hash ok\n");
 	}
	return 0;
}

/*	!!! ak sa užívateľ snaží updatovať funkciu s rovnakým názvom a počtom parametrov nestane sa nič !!!


*/

int htInsert ( tHTable ptrht, tHTItem* item_ptr ) {
	printf("som v insert\n");
	tHTItem *actual_item=htSearch(ptrht,item_ptr->key);
	printf("pomocny item ok\n");
	if (actual_item){													//ak je item najdeny aktuelizuje data
		printf("item najdeny\n");
		if ((item_ptr->typ==FUNCTION)&&(actual_item->typ==FUNCTION)){	//obe su funkcie
			if (actual_item->param_count==item_ptr->param_count){
				if (item_ptr->defined && !actual_item->defined){
					actual_item->defined=item_ptr->defined;
					return 0;
				}
			}
		}

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
 	else{
 		printf("item nenajdeny ok\n");
 		tHTItem *Ninsert = malloc(sizeof(tHTItem));	//ak nie je najdeny naalokuje si pamat pre dany item
 		if (Ninsert==NULL){							//ak sa alokacia nepodarila tak funkcia skonci
 			return sym_table_error(ER_INTERNAL);
 		printf("new item alokacia ok\n");
 		}

		Ninsert->key = (char*) malloc((strlen((item_ptr->key))+2));
		if (Ninsert->key==NULL){							//ak sa alokacia nepodarila tak funkcia skonci
 			return sym_table_error(ER_INTERNAL);
 		}
 		printf("key alokacia ok\n");

 		strcpy(Ninsert->key,item_ptr->key);
 		printf("nahratie kluca ok\n");
 		Ninsert->typ=item_ptr->typ;							//nahraju sa data
 		Ninsert->param_count=item_ptr->param_count;
 		Ninsert->defined=item_ptr->defined;		
 		Ninsert->ptrnext=(ptrht)[hashCode(item_ptr->key)];	//prvok zaradime na zaciatok zoznamu
 		(ptrht)[hashCode(item_ptr->key)]=Ninsert;
 	}
 return 0;
}



Type_of_tHTItem* get_type (tHTable ptrht,char key[]) {
/*
	printf("som v get_type\n");
	printf("%s\n",key);
	tHTItem *tmp=(htSearch(ptr,key));
	//printf("%s\n",tmp->key);
									
	return (tmp->typ);
	*/
	printf("som v get_type\n");
	printf("%s\n",key);
	if (htSearch(ptrht,key)!=NULL){				//ak je item najdeny
		return &(htSearch(ptrht,key))->typ;	//funkcia vrati data hladaneho itemu
	}
	else{										//inak vrati NULL
		return NULL;
	}
}


int check_define (char key[]) {		//defined==TRUE==1 ; defined==FALSE==0; not_a_function==2; NOT_FOUND==3

	tHTItem *tmp = htSearch(global_ST,key);
	if (tmp==NULL){
		return 3;
	}

	if ((tmp->typ)!=FUNCTION){
		return 2;				
	}else{
		return ((tmp->defined));
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
			tmp=(ptrht)[i];		//postupne uvolni cely zoznam
			(ptrht)[i]=(ptrht)[i]->ptrnext;
			free(tmp->key);
			free(tmp);
		}
	}
	htInit(ptrht);					//reinicializacia tabulky
}


int STlast_check(){

	tHTItem *tmp; //pomocny pointer
	
	for (int i=0;i<HTSIZE;i++){	//cyklus prechadza tabulkov
		
		tmp=global_ST[i];

		while(tmp!=NULL){	//cyklus prechadza zoznamom
			printf("%s\n",tmp->key );
			if ((tmp->typ==FUNCTION)&&(tmp->defined==FALSE)){
				return sym_table_error(ER_SEM_VARIABLE);
			}
			tmp=tmp->ptrnext;
		}
	}
	return 0;
}
