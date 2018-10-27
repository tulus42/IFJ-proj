#include "LL-grammar.h"

int get_rule_num(int nonterminal, Token_t* token){
    switch(nonterminal){
        case PROG:
            if(token->token == TYPE_KEYWORD){
                if(token->attr.keyword == KEYWORD_DEF)
                    return 1;
                else if(token->attr.keyword == KEYWORD_IF)
                    return 5;
                else if(token->attr.keyword == KEYWORD_WHILE)
                    return 5;
                else if(token->attr.keyword == KEYWORD_PRINT)
                    return 5;
                else if(token->attr.keyword == KEYWORD_LENGTH)
                    return 5;
                else if(token->attr.keyword == KEYWORD_SUBSTR)
                    return 5;
                else if(token->attr.keyword == KEYWORD_ORD)
                    return 5;
                else if(token->attr.keyword == KEYWORD_CHR)
                    return 5;
            }
            else if(token->token == TYPE_EOL)
                return 2;
            else if(token->token == TYPE_IDENTIFIER)
                return 3;
            else if(token->token == TYPE_EOF)
                return 4;
            return 0;
            break;
        case STATEMENT:
            if(token->token == TYPE_EOL)
                return 17;
            else if(token->token == TYPE_KEYWORD){
                if(token->attr.keyword == KEYWORD_IF)
                    return 10;
                else if(token->attr.keyword == KEYWORD_WHILE)
                    return 11;
                else if(token->attr.keyword == KEYWORD_PRINT)
                    return 12;
                else if(token->attr.keyword == KEYWORD_LENGTH)
                    return 13;
                else if(token->attr.keyword == KEYWORD_SUBSTR)
                    return 14;
                else if(token->attr.keyword == KEYWORD_ORD)
                    return 15;
                else if(token->attr.keyword == KEYWORD_CHR)
                    return 16;
            }
            return 0;
            break;
        case VALUE:
            if(token->token == TYPE_INT)
                return 6;
            else if(token->token == TYPE_STRING)
                return 7;
            else if(token->token == TYPE_STRING)
                return 8;
            return 0;
            break;
        case VARIABLE:
            break;
        case DECLARE:
            break;
    }
        
}

void get_rule(){
    ;
}