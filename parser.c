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
#include "symtable.h"

#include "lexer.c"
#include "parser.h"


#define GET_TOKEN()                                     \
    if (get_next_token(data->token) != LEXER_OK)        \
        return(ER_LEX)

/**
 * TODO: 
 * nový main
 * kontrola 0 alebo 1 LEX. analýzy
 * allocation of tokens
 * structures for parser.h
 * procedenční tabuľka
 * LL-gramatika
 */

// forward declarations
static int statement(Data_t* data);
static int declare(Data_t* data);
static int params(Data_t* data);
static int param(Data_t* data);
static int argvs(Data_t* data);
static int arg(Data_t* data);
static int value(Data_t* data);


// Frees all the memory
int parser_error(Data_t* Data, string_t* string ){
    // frees the memory
    free_string(string);
    free(Data->token);

    return ER_SYN;
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

    printf("In <prog>\nToken: %s\n", tokens[data->token->token]);

    // <prog> -> DEF ID_FUNC ( <params> ) EOL <statement> END <prog>
    if(data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_DEF){
        // ... ID_FUNC ...
        GET_TOKEN();
        // somehow check ID_FUNC in table
        

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
        
        // ... <statement> ...
        data->in_definition = true;
        GET_TOKEN();
        if (statement(data) != SYN_OK) 
            return(ER_SYN);
        
        // ... END ...  - nevolame GET_TOKEN, pretoze sem sa vrati zo <statement> len ak uz token == END
        if (!(data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_END))
            return(ER_SYN);

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


    // <prog> -> <statements> <prog> ???? toto treba ešte domyslieť
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

    
    // <statement> -> PRINT ( <argvs> ) EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_PRINT) {
        printf("in <statement> PRINT\n");
        GET_TOKEN();
        if (1/*<argvs>*/) {                    // TODO <argvs>
            ;
        } else
            return(ER_SYN);
        
         // ... EOL || EOF ... 
        GET_TOKEN();
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF)   
            return(prog(data));
    }


    // <statement> -> LENGTH ( <argvs> ) EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_LENGTH) {
        printf("in <statement> LENGTH\n");
        GET_TOKEN();
        if (1/*<argvs>*/) {                    // TODO <argvs>
            ;
        } else
            return(ER_SYN);
    
         // ... EOL || EOF ... 
        GET_TOKEN();
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF)   
            return(prog(data));
    }


    // <statement> -> SUBSTR ( <argvs> ) EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_SUBSTR) {
        printf("in <statement> SUBSTR\n");
        GET_TOKEN();
        if (1/*<argvs>*/) {                    // TODO <argvs>
            ;
        } else
            return(ER_SYN);
        
         // ... EOL || EOF ... 
        GET_TOKEN();
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF)   
            return(prog(data));
    }


    // <statement> -> ORD ( <argvs> ) EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_ORD) {
        printf("in <statement> ORD\n");
        GET_TOKEN();
        if (1/*<argvs>*/) {                    // TODO <argvs>
            ;
        } else
            return(ER_SYN);
        
         // ... EOL || EOF ... 
        GET_TOKEN();
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF)   
            return(prog(data));
    }


    // <statement> -> CHR ( <argvs> ) EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_CHR) {
        printf("in <statement> CHR\n");
        GET_TOKEN();
        if (1/*<argvs>*/) {                    // TODO <argvs>
            ;
        } else
            return(ER_SYN);

         // ... EOL || EOF ... 
        GET_TOKEN();
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF)   
            return(prog(data));
    }

    // <statement> -> INPUTS EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_INPUTS) {
        printf("in <statement> INPUTS\n");
    
         // ... EOL || EOF ... 
        GET_TOKEN();
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF)   
            return(prog(data));
    }

    
    // <statement> -> INPUTI EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_INPUTI) {
        printf("in <statement> INPUTI\n");
    
         // ... EOL || EOF ... 
        GET_TOKEN();
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF)   
            return(prog(data));
    }

    // <statement> -> INPUTF EOL
    else if (data->token->token == TYPE_KEYWORD && data->token->attr.keyword == KEYWORD_INPUTF) {
        printf("in <statement> INPUTF\n");
    
         // ... EOL || EOF ... 
        GET_TOKEN();
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF)   
            return(prog(data));
    }

    // <statement> -> ID <declare> EOL
    else if (data->token->token == TYPE_IDENTIFIER) {
        printf("in <statement> ID <declare>\n");
        
        // somehow check ID in table
        // I have no idea how

        // ... <declare> ...
        GET_TOKEN();
        if (declare(data) != SYN_OK)
            return(ER_SYN);
        
        //
        // ... EOL || EOF ... 
        GET_TOKEN();
        if (data->token->token == TYPE_EOL || data->token->token == TYPE_EOF)   
            return(prog(data));
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


static int declare(Data_t* data) {
// <declare> -> = ID
// <declare> -> = <value>
// <declare> -> = <expression>
// <declare> -> = ID_FUNC ( <argvs> )
// <declare> -> ε
}

static int params(Data_t* data) {
// <params> -> ID <param>

// somehow check ID in table                             TODO
if (data->token->token == TYPE_IDENTIFIER) {
    GET_TOKEN();
    return(param(data));
} else
if (data->token->token == TYPE_RIGHT_BRACKET) {
    return(SYN_OK);
}


// <params> -> ε
}

static int param(Data_t* data) {
// <param> , ID <param>

// <param> -> ε
}

static int argvs(Data_t* data) {
// <argvs> -> <value> <arg>
// <argvs> -> ε
}

static int arg(Data_t* data) {
// <arg> -> , <value> <arg>
// <arg> -> ε
}

static int value(Data_t* data) {
// <value> -> INT_VALUE
// <value> -> FLOAT_VALUE
// <value> -> STRING_VALUE
}



static bool init_struct(Data_t* data){
    data->token = malloc(sizeof(Token_t));
    data->token->token = 100;
    data->in_function = false;
    data->in_while_or_if = 0;
    data->in_definition = false;

    return true;
}

int start_parser(){

    Data_t our_data;
    string_t string;

    allocate_string(&string);
    set_string(&string);

    if(!init_struct(&our_data)){
        free_string(&string);
    }
    
    
    int res;
    res = prog(&our_data);

    
    free_string(&string);
    free(our_data.token);

    if (our_data.in_while_or_if != 0)
        return(ER_SYN);
    else
        return res;
    //return 0;
}







