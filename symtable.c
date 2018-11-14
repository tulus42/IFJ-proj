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


/**
 * Writes error to stderr, returns error code
 */
int sym_table_error(int error_code){
	htClearAll(local_ST);
	//htInit(local_ST);
	htClearAll(global_ST);
	//htInit(global_ST);
	fprintf(stderr, "sym_table ERROR\n");
return error_code;
}


/*Funkcia vracia index tabulky na zaklade kluca
**Funkcia je prebraná z 2. domácej úlohy IAL
*/
int hashCode ( char key[] ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}


/*Funkcia inicializuje kazdú položku tabuľky na NULL
**Využíva sa len vo funkciách htInits() a htClearAll()
*/

void htInit ( tHTable ptrht ) {

	for (int i=0;i<HTSIZE;i++){
			(ptrht)[i]=NULL;
		}
}

/*
** Funkcia inicializuje obe tabulky
*/
void STinits(){
	htInit(global_ST);
	htInit(local_ST);
}

/* Hashovacia tabuľka s explicitne zreťazenými synonymami.
** Vyhľadávanie prvku v hashovacej tabuľke ptrht podľa zadaného kľúča key. 
** Ak je daný prvok nájdený, vracia pointer na daný prvok. 
** Ak sa položka v tabuľke nenachádza funkcia vracia NULL.
**
*/
tHTItem* htSearch ( tHTable ptrht, char key[] ) {

	tHTItem *tmp=(ptrht)[hashCode(key)];

	while (tmp){							
	
		if (strcmp(tmp->key, key)==0){		
			return tmp;
		}
		if (tmp->ptrnext==NULL){			
			return NULL;
		}
		else{
			tmp=tmp->ptrnext;				
		}
	}
return NULL;
}

/* Funkcia vkladá do zvolenej tabuľky ptrht položku key, ktorá má typ NILL
** Pri chybe alokácie vracia hodnotu ER_INTERNAL
** Pri pokuse o redefiniciu premennej vracia ER_SEM_VARIABLE
** Inak vracia 0
*/

int def_ID( tHTable ptrht,char key[] ){

	if (htSearch(ptrht,key)!=NULL){							//ak je položka nájdená jedná sa o ER_SEM_VARIABLE
		
		return sym_table_error(ER_SEM_VARIABLE);
 		 
 	}
 	else{
 		tHTItem *Ninsert = malloc(sizeof(tHTItem));			//alokácia novej položky
 		if (Ninsert==NULL){								
 			return sym_table_error(ER_INTERNAL);

 		}
 		
		Ninsert->key = (char*) malloc((strlen(key)+2));		//alokácia kľúča
		if (Ninsert->key==NULL){							
 			return sym_table_error(ER_INTERNAL);
 		}

 		strcpy(Ninsert->key,key);							//inicializácia položky
 		Ninsert->typ=NILL;	
 		Ninsert->param_count=0;
 		Ninsert->defined=FALSE;						
 		Ninsert->ptrnext=(ptrht)[hashCode(key)];			//prvok zaradime na zaciatok zoznamu
 		(ptrht)[hashCode(key)]=Ninsert;
 	}
	return 0;
}

/* Funkcia vkladá do zvolenej tabuľky ptrht celú vybranú položku item_ptr 
** Ak je položka už v tabuľke aktualizuje jej typ 
** v pripade FUNCTION môže preklopiť jej defined zložku
*/

int htInsert ( tHTable ptrht, tHTItem* item_ptr ) {
	printf("som v insert\n");
	tHTItem *actual_item=htSearch(ptrht,item_ptr->key);
	printf("pomocny item ok\n");
	if (actual_item){													//ak je item najdeny aktuelizuje data
		printf("item najdeny\n");
		if ((item_ptr->typ==FUNCTION)&&(actual_item->typ==FUNCTION)){	//obe su funkcie
			if (actual_item->param_count==item_ptr->param_count){		//maju rovnaky pocet parametrov
				if (item_ptr->defined && !actual_item->defined){		//item v tabzľke je nedefinovany a "pridávaný" item je definovany
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
				case PRASATKO_S_PAPUCKAMI_FLT:
					actual_item->typ= item_ptr->typ;
					//actual_item->data.PRASATKO_S_PAPUCKAMI_FLT=item_ptr->data.PRASATKO_S_PAPUCKAMI_FLT;
					break;
				default:
					return  sym_table_error(ER_SEM_TYPE);
 					break;
 		}
 	}
 	else{																//položka nie je v tabuľke

 		tHTItem *Ninsert = malloc(sizeof(tHTItem));						//alokácia položky
 		if (Ninsert==NULL){							
 			return sym_table_error(ER_INTERNAL);
 		}

		Ninsert->key = (char*) malloc((strlen((item_ptr->key))+2));		//alokácia kľúča
		if (Ninsert->key==NULL){							
 			return sym_table_error(ER_INTERNAL);
 		}

 		strcpy(Ninsert->key,item_ptr->key);								//nahrajú sa kľúč a data
 		Ninsert->typ=item_ptr->typ;							
 		Ninsert->param_count=item_ptr->param_count;
 		Ninsert->defined=item_ptr->defined;		
 		Ninsert->ptrnext=(ptrht)[hashCode(item_ptr->key)];				//prvok zaradime na zaciatok zoznamu
 		(ptrht)[hashCode(item_ptr->key)]=Ninsert;
 	}
 return 0;
}

/* Funkcia vyhľadáva v zvolenej tabuľke ptrht položku s kľúčom key
** Ak je položka nájdená vracia ukazateľ na jej typ
** Ak sa položka v tabuľke nenacjhádza vracia ukazateľ na NULL
*/

Type_of_tHTItem* get_type (tHTable ptrht,char key[]) {

	printf("som v get_type\n");
	printf("%s\n",key);
	if (htSearch(ptrht,key)!=NULL){				//ak je item najdeny
		return &(htSearch(ptrht,key))->typ;		//funkcia vrati typ hladaneho itemu
	}
	else{										//inak vrati NULL
		return NULL;
	}
}

/* Funkcia kontroluje či je funkcia s kľúčom key v globálnej tabuľke symbolov (global_ST) definovaná
** Návratové hodnoty
** NOT_FOUND==3
** not_a_function==2
** defined==FALSE==0
** defined==TRUE==1
*/

int check_define (char key[]) {

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

/* Vyčistí tabuľku ptrht a reinicializuje ju
*/

void htClearAll ( tHTable ptrht ) {



	tHTItem *tmp;
	for (int i=0;i<HTSIZE;i++){
		
		tmp=(ptrht)[i];

		while((ptrht)[i]!=NULL){
			tmp=(ptrht)[i];
			(ptrht)[i]=(ptrht)[i]->ptrnext;
			free(tmp->key);
			free(tmp);
		}
	}
	htInit(ptrht);							//reinicializacia tabulky
}

/* Skontroluje či sú všetky funkcie v Globálnej tabuľke symbolov definovane
** Ak narazí na nedefinovanú funkciu skončí s návratovou hodnotou ER_SEM_VARIABLE
** Ak sú všetky funkcie definované vracia hodnotu 0
*/

int STlast_check(){

	tHTItem *tmp;
	
	for (int i=0;i<HTSIZE;i++){	
		
		tmp=global_ST[i];

		while(tmp!=NULL){
			printf("%s\n",tmp->key );
			if ((tmp->typ==FUNCTION)&&(tmp->defined==FALSE)){
				return sym_table_error(ER_SEM_VARIABLE);
			}
			tmp=tmp->ptrnext;
		}
	}
	return 0;
}
