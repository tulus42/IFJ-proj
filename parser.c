/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include "error.h"
#include "lexer.c"


#include "parser.h"


#define GET_TOKEN()                                                          \
    if (get_next_token(data->token) != LEXER_OK)                             \
        return(ER_LEX)

#define IS_VALUE()                                                           \
    data->token->token == TYPE_INT || data->token->token == TYPE_FLOAT      \
    || data->token->token == TYPE_STRING || data->token->token == TYPE_IDENTIFIER 
        



/**
 * TODO: 
 * 
 * vyriešiť situáciu "func ()/func param" - bez zátvoriek
 * dorobiť kontrolu premenných a funkcií v tabulke symbolov
 * urobiť <declare>
 * procedenční tabuľka
 * 
 */

// premenna pre uchovanie ID z tokenu pre neskorsie ulozenie do TS
string_t identifier;

// forward declarations
static int statement(Data_t* data);
static int declare(Data_t* data);
static int params(Data_t* data);
static int param(Data_t* data);
static int argvs(Data_t* data);
static int arg(Data_t* data);
static int value(Data_t* data);
static int function(Data_t* data);


// Frees all the memory
int parser_error(Data_t* Data, string_t* string ){
    // frees the memory
    free_string(string);
    free(Data->token);

    return ER_SYN;
}

void save_id(string_t* identifier, Data_t* data) {
    add_string(identifier, data->token->attr.string->s);
}

// Kontrola uspesnosti lexikalnej analyzy
int get_token(Data_t* data) {
    if (get_next_token(data->token) == LEXER_OK)
        return(1);
    else
        return(0);
}

/* ****************************
 * <prog>
 * ***************************/
static int prog(Data_t* data){
    int result;

    GET_TOKEN();

    printf("In <prog>, Token: %s\n", tokens[data->token->token]);

    // <prog> -> DEF ID_FUNC ( <params> ) EOL <statement> END <prog>
    if(data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_DEF){
        if (data->in_definition == false) {
            data->in_definition = true;
        } else {
            return(ER_SYN);
        }

        // ... ID_FUNC ...
        GET_TOKEN();
        save_id(&identifier, data);
        printf("ID: %s\n", identifier.s);
        /*
         * somehow check ID_FUNC in table
         */

        // ... ( ...
        GET_TOKEN();
        if (data->token->token != TYPE_LEFT_BRACKET) 
            return(ER_SYN);
        
        // ... <params> ...
        GET_TOKEN();
        if (params(data) != SYN_OK) 
            return(ER_SYN);
        
        // ... ) ... - nevolame GET_TOKEN
        if (data->token->token != TYPE_RIGHT_BRACKET) 
            return(ER_SYN);
    
        // ... EOL ...
        GET_TOKEN();
        if (data->token->token != TYPE_EOL) 
            return(ER_SYN);
        
        printf("Checkpoint 1\n");

        // ... <statement> ...
        GET_TOKEN();
        if (statement(data) != SYN_OK) 
            return(ER_SYN);
        
        printf("Checkpoint 2\n");

        // ... END ...  - nevolame GET_TOKEN, pretoze sem sa vrati zo <statement> len ak uz token == END
        if (!(data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_END))
            return(ER_SYN);

        printf("Checkpoint 3\n");

        // ... EOL || EOF ... 
        GET_TOKEN();
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF)   
            return(prog(data));
    } 


    // <prog> -> EOL <prog>
    else if(data->token->token == TYPE_EOL){
        printf("Token in EOL loop: %d\n",data->token->token);
        return(prog(data));
    } 


    // <prog> -> EOF
    else if(data->token->token == TYPE_EOF){
        printf("EOF - exit\n");
        return SYN_OK;
    } 


    // <prog> -> <statements> <prog>
    else if(data->token->token == TYPE_KEYWORD || data->token->token == TYPE_IDENTIFIER){
        printf("INTO <statement> Token: %s\n", tokens[data->token->token]);
        int res=statement(data);
        printf("Return value of <statement> is: %d\n", res);
        return res;
        //return(statement(data));  - pouzijeme toto, ale kvoli debuggingu je to rozpisane
    }
    
        
    return ER_SYN;
}

/* ****************************
 * <statement>
 * ***************************/
static int statement(Data_t* data) {
    printf("In <statement>, in_while_of_if: %d\n", data->in_while_or_if);
    printf("Token: %s\n", tokens[data->token->token]);
    // printf("%s\n", data->token->attr.string->s);
    // <statement> -> IF <expression> THEN EOL <statements> ELSE EOL <statements> END EOL
    // ... IF ...
    if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_IF) {
        data->in_while_or_if++;
        // ... <expression> ...
        GET_TOKEN();
        printf("Check expression\n");
        if (1/*__expression__*/) {                                   // TODO expression
            ;
        } else 
            return(ER_SYN);
        
        // ... THEN ...
        GET_TOKEN();
        printf("Check THEN\n");
        if (data->token->token != TYPE_KEYWORD || data->token->attr.keyword != KEYWORD_THEN) {
            printf("ERR1\n");
            return(ER_SYN);
        }
    
        // ... EOL ...
        GET_TOKEN();
        printf("Check EOL\n");
        if (data->token->token != TYPE_EOL) {
            printf("ERR2\n");
            return(ER_SYN);
        }

        // ... <statements> ...
        GET_TOKEN();
        printf("Check next statement\n");
        if (statement(data) != SYN_OK){
            printf("ERR3\n");
            return(ER_SYN);
        }
    
        // ... ELSE ... || ... EN rozsirenie - volitelna cast ELSE
        // ELSE
        printf("Check ELSE\nToken: %s\n", tokens[data->token->token]);
        if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_ELSE) {
            // ... EOL ...
            GET_TOKEN();
            printf("Check EOL\n");
            if (data->token->token != TYPE_EOL) {
                printf("ERR4\n");
                return(ER_SYN);
            }
          
            // ... <statements> ...
            GET_TOKEN();
            printf("Check next statement\n");
            if (statement(data) != SYN_OK){
                printf("ERR5\n");
                return(ER_SYN);
            }
        }
        
        // ... END ...  - nevolame get_token(), pretoze sem sa vrati zo <statement> len ak uz token == END
        if (!(data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_END))
            return(ER_SYN);

        // ... EOL || EOF ... 
        GET_TOKEN();
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF)   
            return(prog(data));
        

        // if nothing good after IF, return ERROR  
        printf("ERR7\n");
        return(ER_SYN);
    }
    

    // <statement> -> WHILE <expression> DO EOL <statement> END EOL
    // ... WHILE ...
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_WHILE) {
        data->in_while_or_if++;
        // ... <expression> ...
        GET_TOKEN();
        if (1/*__expression__*/) {      // TODO expression
            ;
        } else 
            return(ER_SYN);
        
        // ... DO ...
        GET_TOKEN();
        if (data->token->token != TYPE_KEYWORD || data->token->attr.keyword != KEYWORD_DO)
            return(ER_SYN);
    
        // ... EOL ...
        GET_TOKEN();
        if (data->token->token != TYPE_EOL)
            return(ER_SYN);
        
        // ... <statements> ...
        GET_TOKEN();
        printf("Check next statement\n");
        if (statement(data) != SYN_OK)
            return(ER_SYN);
        
        // ... END ...  - nevolame get_token(), pretoze sem sa vrati zo <statement> len ak uz token == END
        if (!(data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_END))
            return(ER_SYN);

        // ... EOL || EOF ... 
        GET_TOKEN();
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF)   
            return(prog(data));
    

        // if nothing good after WHILE, return ERROR
        return(ER_SYN);
    }

    // <statement> -> <function> EOL
    // pre vstavené funkcie
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword > 8 && data->token->attr.keyword < 17) {
        printf("in <statement> -> <function>\n");
        // ... <function> ...
        if (function(data) != SYN_OK)
            return(ER_SYN);

        // ... EOL || EOF ... - token nepytame, bude vdaka nemu navratena <function>
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF)   
            return(prog(data));
    }
    

    // <statement> -> ID EOL
    // <statement> -> ID <declare> EOL
    // <statement> -> ID_FUNC ( <argvs> ) EOL
    else if (data->token->token == TYPE_IDENTIFIER) {

        // <statement> -> ID EOL
        // <statement> -> ID <declare> EOL


        // if (ID_var) then:    ************************
        printf("in <statement> ID EOL/ID <declare>/ID_FUNC...\n");
        save_id(&identifier, data);
        //*** somehow check ID in table

        GET_TOKEN();
        // <statement> -> ID <declare> EOL
        // ... = ...
        if (data->token->token == TYPE_ASSIGN) {
            return(declare(data));
        } else

        // <statement> -> ID EOL || EOF 
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF) {
            /**
             * check if ID in table
             */
            return(prog(data));
        } else
        

        // else if (ID_func) then :************************
        // <statement> -> ID_FUNC ( <argvs> )
        if (data->token->token == TYPE_LEFT_BRACKET) {
            /******
             * check if ID_FUNC in table
             * if (in table) {
             *    vsetko ok a pokracuj
             * } else {
             *    if (data->in_declaration == true) {
             *       save ID_FUNC in table, ID_FUNC.declared = false;
             *    } else
             *       return(ER_SYN);
             * }
             */
           
            GET_TOKEN();
            return(argvs(data));
        } else {

            return(ER_SYN);
        }
    }

    

    // <statement> -> EOL <prog>
    // povoluje brat prazdne riadky ako statement - napr IF () THEN 2xEOL END
    if (data->token->token == TYPE_EOL)
        return(prog(data));

    // ****************************************
    // make <statement> return when END or ELSE
    if (data->token->token == TYPE_KEYWORD && (data->token->attr.keyword == KEYWORD_END || data->token->attr.keyword == KEYWORD_ELSE)) {
        // ak ide o IF/WHILE
        if (data->in_while_or_if != 0) {
            printf("Check in_while_or_if\n");
            if (data->token->attr.keyword == KEYWORD_END) {

                data->in_while_or_if--;
                printf("Return <statement>2\n");
                return(SYN_OK);
            }
            if (data->token->attr.keyword == KEYWORD_ELSE) {
                printf("Return <statement>1\n");
                return(SYN_OK);
            }
        } else 
        // ak ide o DEFINITION
        if (data->in_definition == true) {
            if (data->token->attr.keyword == KEYWORD_END) {
                data->in_definition = false;
                printf("Return <statement>3\n");
                return(SYN_OK);
            }
        } else
            return(ER_SYN);
    }



    printf("End <statemtnt>\n");
    return(ER_SYN);
}

/*
    if (get_token(data)) {

    } else
        return(ER_LEX);
*/


/* ****************************
 * <declare>
 * ***************************/
static int declare(Data_t* data) {
    // <declare> =

    GET_TOKEN();

    // <declare> -> = <value>
    // <declare> -> = <expression>
    if (data->token->token == TYPE_IDENTIFIER) {

    } else {}

    // <declare> -> = ID_FUNC ( <argvs> )
    // <declare> -> ε
}


/* ****************************
 * <params>
 * ***************************/
static int params(Data_t* data) {
    // <params> -> ID <param>
    // ... ID ...
    if (data->token->token == TYPE_IDENTIFIER) {
        printf("ID: %s\n",data->token->attr.string->s);
        int res;
        res = param(data);
        printf("<params> return: %d\n", res);
        return(res);
        //return(param(data));
    } else

    // <params> -> ε
    // ... ) ... - nepytame TOKEN - len pripad f()
    if (data->token->token == TYPE_RIGHT_BRACKET) {
        return(SYN_OK);
    }

    return(ER_SYN);
}

static int param(Data_t* data) {
    // <param> , ID <param>
    // ... ID ... - znovu musime kontrolovat ID, kvoli rekurzivnemu volaniu
    if (data->token->token == TYPE_IDENTIFIER) {
        printf("<param> ID: %s\n", data->token->attr.string->s);
    /**
     * somehow check ID in table
     * 
     **/
    
        GET_TOKEN();
        // ... , ...
        if (data->token->token == TYPE_COMMA) {

            GET_TOKEN();
            return(param(data));
        
        } else
        // ... ) ...
        if (data->token->token == TYPE_RIGHT_BRACKET) {
            return(params(data));
        }
    }

    // <param> -> ε
}


/* ****************************
 * <argvs>
 * ***************************/
static int argvs(Data_t* data) {
    // <argvs> -> <value> <arg>
    // ... <value> ...

    printf("in <argvs>\n");
    if (data->token->token != TYPE_RIGHT_BRACKET) {
    

        if (arg(data) != SYN_OK) {
            return(ER_SYN);
        }
        return(prog(data));

    // ... ) ...
    } else {
        return(prog(data));
    }
    // <argvs> -> ε
}

static int arg(Data_t* data) {
    // <arg> -> , <value> <arg>
    // <arg> -> ε

    printf("<arg>\n");
    if (IS_VALUE()) {
        printf("in IS_VALUE\n");
    /**
     * somehow check ID in table
     * 
     **/
    
        GET_TOKEN();
        // ... , ...
        if (data->token->token == TYPE_COMMA) {

            GET_TOKEN();
            return(arg(data));
        
        } else
        // ... ) ...
        if (data->token->token == TYPE_RIGHT_BRACKET) {
            return(argvs(data));
        }
    }
}


/* ****************************
 * <value>
 * ***************************/
static int value(Data_t* data) {
    // <value> -> INT_VALUE
    if (data->token->token == TYPE_INT){

    }
    // <value> -> FLOAT_VALUE
    if (data->token->token == TYPE_FLOAT){

    }
    // <value> -> STRING_VALUE
    if (data->token->token == TYPE_STRING){

    }
}


/* ****************************
 * <function>
 * ***************************/
static int function(Data_t* data) {
    printf("in <function>\n");
    // <function> -> PRINT ( <argvs> ) EOL
    if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_PRINT) {
        printf("in <function> PRINT\n");
        GET_TOKEN();
        if (1/*<argvs>*/) {                    // TODO <argvs>
            ;
        } else
            return(ER_SYN);
        
        // ... EOL || EOF ... 
        GET_TOKEN();
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF) {  
            printf("Checkpoint 4 - successfull PRINT\n");
            return(SYN_OK);
        } else {
            return(ER_SYN);
        }
    }


    // <function> -> LENGTH ( <argvs> ) EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_LENGTH) {
        printf("in <function> LENGTH\n");
        GET_TOKEN();
        if (1/*<argvs>*/) {                    // TODO <argvs>
            ;
        } else
            return(ER_SYN);
    
        // ... EOL || EOF ... 
        GET_TOKEN();
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF)   
            return(SYN_OK);
        else    
            return(ER_SYN);
    }


    // <function> -> SUBSTR ( <argvs> ) EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_SUBSTR) {
        printf("in <function> SUBSTR\n");
        GET_TOKEN();
        if (1/*<argvs>*/) {                    // TODO <argvs>
            ;
        } else
            return(ER_SYN);
        
        // ... EOL || EOF ... 
        GET_TOKEN();
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF)   
            return(SYN_OK);
        else    
            return(ER_SYN);
    }


    // <function> -> ORD ( <argvs> ) EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_ORD) {
        printf("in <function> ORD\n");
        GET_TOKEN();
        if (1/*<argvs>*/) {                    // TODO <argvs>
            ;
        } else
            return(ER_SYN);
        
        // ... EOL || EOF ... 
        GET_TOKEN();
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF)   
            return(SYN_OK);
        else    
            return(ER_SYN);
    }


    // <function> -> CHR ( <argvs> ) EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_CHR) {
        printf("in <function> CHR\n");
        GET_TOKEN();
        if (1/*<argvs>*/) {                    // TODO <argvs>
            ;
        } else
            return(ER_SYN);

        // ... EOL || EOF ... 
        GET_TOKEN();
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF)   
            return(SYN_OK);
        else    
            return(ER_SYN);
    }

    // <function> -> INPUTS EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_INPUTS) {
        printf("in <function> INPUTS\n");
    
         // ... EOL || EOF ... 
        GET_TOKEN();
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF)   
            return(SYN_OK);
        else    
            return(ER_SYN);
    }

    
    // <function> -> INPUTI EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_INPUTI) {
        printf("in <function> INPUTI\n");
    
         // ... EOL || EOF ... 
        GET_TOKEN();
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF)   
            return(SYN_OK);
        else    
            return(ER_SYN);
    }

    // <function> -> INPUTF EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_INPUTF) {
        printf("in <function> INPUTF\n");
    
         // ... EOL || EOF ... 
        GET_TOKEN();
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF)   
            return(SYN_OK);
        else    
            return(ER_SYN);
    }
}

static bool init_struct(Data_t* data){
    data->token = malloc(sizeof(Token_t));
    data->token->token = 100;
    data->in_function = false;
    data->in_while_or_if = 0;
    data->in_definition = false;

    return true;
}


/**
 * PARSER
 **/
int start_parser(){

    Data_t our_data;
    string_t string;

    allocate_string(&string);
    set_string(&string);

    if(!init_struct(&our_data)){
        free_string(&string);
    }

    //identifier = (char *)malloc(sizeof(char));
    allocate_string(&identifier);

    /*
    tstackP *s;
    FrameStackInit(s);
    */

    int res;
    res = prog(&our_data);

    
    free_string(&string);
    free(our_data.token);

    //free(identifier);
    free_string(&identifier);

    /*
    DeleteStack(s);
    */

    if (our_data.in_while_or_if != 0)
        return(ER_SYN);
    else
        return res;
    //return 0;
}







