/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

// VARIANTA HASH TABLE

#include "symtable.h"


const char* types_of_HTitem[] = {
	"NILL",
	"STRING",
	"INTEGER",
	"PRASATKO_S_PAPUCKAMI_FLT",
	"VAR",
	"FUNCTION",
};

/**
 ** Returns error code
 */

int sym_table_error(int error_code){
	htClearAll(local_ST);
	htClearAll(global_ST);
return error_code;
}


/*
** Function returns idex of item based on key
** Function is borowed from IAL 2. Home work
*/
int hashCode ( char key[] ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}


/*
** Function inits every index of chosen table on NULL
** Function is used in htInits() and htClearAll()
*/

void htInit ( tHTable ptrht ) {

	for (int i=0;i<HTSIZE;i++){
			(ptrht)[i]=NULL;
		}
}

/*
** Function inits both tables
*/
void STinits(){
	htInit(global_ST);
	htInit(local_ST);
}

/* 
** Hash table with explicitly linked synonyms
** Function search in chosen table for item based on key 
** Function returns pointer on item if result is positive
** Function returns NULL if result is negative
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

/* 
** Function inserts item with chosen key and type VAR to chosen table
** If alocation false function returns ER_INTERNAL
** If item is redifiend, funtion returns ER_SEM_VARIABLE
** Else returns 0
*/

int def_ID( tHTable ptrht,char key[] ){

	if (htSearch(ptrht,key)!=NULL){							// If search result is positive function returns ER_SEM_VARIABLE
		
		return sym_table_error(ER_SEM_VARIABLE);
 		 
 	}
 	else{
 		tHTItem *Ninsert = malloc(sizeof(tHTItem));			// new alocation
 		if (Ninsert==NULL){								
 			return sym_table_error(ER_INTERNAL);

 		}
 		
		Ninsert->key = (char*) malloc((strlen(key)+2));		// key alocation
		if (Ninsert->key==NULL){							
 			return sym_table_error(ER_INTERNAL);
 		}

 		strcpy(Ninsert->key,key);							// item init
 		Ninsert->typ=VAR;	
 		Ninsert->param_count=0;
 		Ninsert->defined=true;						
 		Ninsert->ptrnext=(ptrht)[hashCode(key)];			// item is add to start of list
 		(ptrht)[hashCode(key)]=Ninsert;
 	}
	return ST_OK;
}

/* 
** Function inserts item_ptr to chosen table
** If item alredy exists in table function can update its defined atribute
*/

int htInsert ( tHTable ptrht, tHTItem* item_ptr ) {


	tHTItem *actual_item=htSearch(ptrht,item_ptr->key);
	if (actual_item){													// If item exists
		if ((item_ptr->typ==FUNCTION)&&(actual_item->typ==FUNCTION)){	// Both items are functions
			if (actual_item->param_count==item_ptr->param_count){		// They have same number of parameters
				if (item_ptr->defined && !actual_item->defined){		// If found item is defined and item_ptr is undefined
					actual_item->defined=item_ptr->defined;
					return ST_OK;
				}
			} else {
				return(ER_SEM_PARAMETERS);
			}
		} 
		if (item_ptr->defined == true) {								// Define item if item_ptr is defined
			actual_item->defined= true;
			return(ST_OK);
		}
 	}
 	else{																// if item isn t in table

 		tHTItem *Ninsert = malloc(sizeof(tHTItem));						// item alocation
 		if (Ninsert==NULL){							
 			return sym_table_error(ER_INTERNAL);
 		}

		Ninsert->key = (char*) malloc((strlen((item_ptr->key))+2));		// key alocation
		if (Ninsert->key==NULL){							
 			return sym_table_error(ER_INTERNAL);
 		}

 		strcpy(Ninsert->key,item_ptr->key);								// save key and datá
 		Ninsert->typ=item_ptr->typ;							
 		Ninsert->param_count=item_ptr->param_count;
 		Ninsert->defined=item_ptr->defined;		
 		Ninsert->ptrnext=(ptrht)[hashCode(item_ptr->key)];				// item is add to start of list
 		(ptrht)[hashCode(item_ptr->key)]=Ninsert;
 	}
 return ST_OK;
}

/* 
** Function search in chosen table for key
** If item is found function returns pointer on its type
** Else returns NULL
*/

Type_of_tHTItem* get_type (tHTable ptrht,char key[]) {

	if (htSearch(ptrht,key)!=NULL){				// if item is found
		return &(htSearch(ptrht,key))->typ;		// returns pointer on typ of searched item
	}
	else{										// else NULL
		return NULL;
	}
}

/* 
** Function checks if item is VAR/Function and defined/undefined
*/

int check_define (tHTable tab, char key[]) {

	tHTItem *tmp = htSearch(tab, key);
	if (tmp==NULL){
		return NOT_FOUND;
	}

	if ((tmp->typ)!=FUNCTION){
		if (tmp->defined){
			return PARAM_DEFINED;
		}else{
			return PARAM_UNDEFINED;
		}			
	}else{
		if (tmp->defined) 
		{
			return FUNCTION_DEFINED;
		}else 
			return FUNCTION_UNDEFINED;
	}								
}

/* 
** clears table and then init table
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
	htInit(ptrht);							// reinit table
}

/* 
** Checks if functions are defined
** If undefined function is found returns ER_SEM_VARIABLE
** If each function is defined returns 0
*/

int STlast_check(){

	tHTItem *tmp;
	
	for (int i=0;i<HTSIZE;i++){	
		
		tmp=global_ST[i];

		while(tmp!=NULL){
			if ((tmp->typ==FUNCTION)&&(tmp->defined==false)){
				return sym_table_error(ER_SEM_VARIABLE);
			}
			tmp=tmp->ptrnext;
		}
	}
	return ST_OK;
}

void htClearAlltables(){
	htClearAll(local_ST);
	htClearAll(global_ST);

}

void htClearlocal () {



	tHTItem *tmp;
	for (int i=0;i<HTSIZE;i++){
		
		tmp=(local_ST)[i];

		while((local_ST)[i]!=NULL){
			tmp=(local_ST)[i];
			(local_ST)[i]=(local_ST)[i]->ptrnext;
			free(tmp->key);
			free(tmp);
		}
	}
	htInit(local_ST);							//reinit local table
}

int iteminit(tHTItem* item,char k[], Type_of_tHTItem t,bool d, int pc ){

	if (item==NULL){							
		return sym_table_error(ER_INTERNAL);
	}
	item->key = (char*) malloc((strlen(k)+2));
	if (item->key==NULL){								// failed alocation returns ER_INTERNAL
			return sym_table_error(ER_INTERNAL);
	}

	strcpy(item->key,k);
	item->typ=t;	
	item->param_count=pc;
	item->defined=d;						
	item->ptrnext=NULL;
return ST_OK;

}

int itemupdate(tHTItem* item,char k[], Type_of_tHTItem t,bool d, int pc ){
	
	free(item->key);
	item->key = (char*) malloc((strlen(k)+2));
	if (item->key==NULL){								// failed alocation returns ER_INTERNAL

			return sym_table_error(ER_INTERNAL);
	}


	strcpy(item->key,k);
	item->typ=t;	
	item->param_count=pc;
	item->defined=d;						
	item->ptrnext=NULL;
return ST_OK;

}

void itemfree(tHTItem* item){
	free(item->key);
}



int check_param_cnt(char key[]){


	return htSearch(global_ST, key)->param_count;

}
