/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

#include "LL-grammar.h"

int get_rule_num(int nonterminal, Token_t* token){
    switch(nonterminal){
        case PROG:
            if(token->token == TYPE_KEYWORD){
                if(token->attr.keyword == KEYWORD_DEF)
                    return 1;
                else if(token->attr.keyword == KEYWORD_IF)
                    return 4;
                else if(token->attr.keyword == KEYWORD_WHILE)
                    return 4;
                else if(token->attr.keyword == KEYWORD_PRINT)
                    return 4;
                else if(token->attr.keyword == KEYWORD_LENGTH)
                    return 4;
                else if(token->attr.keyword == KEYWORD_SUBSTR)
                    return 4;
                else if(token->attr.keyword == KEYWORD_ORD)
                    return 4;
                else if(token->attr.keyword == KEYWORD_CHR)
                    return 4;
                else if(token->attr.keyword == KEYWORD_INPUTS)
                    return 4;
                else if(token->attr.keyword == KEYWORD_INPUTI)
                    return 4;
                else if(token->attr.keyword == KEYWORD_INPUTF)
                    return 4;
            }
            else if(token->token == TYPE_EOL)
                return 2;
            else if(token->token == TYPE_IDENTIFIER)
                return 4;
            else if(token->token == TYPE_EOF)
                return 3;
            return 0;
            break;
        case STATEMENT:
            if(token->token == TYPE_KEYWORD){
                if(token->attr.keyword == KEYWORD_IF)
                    return 5;
                else if(token->attr.keyword == KEYWORD_WHILE)
                    return 6;
                else if(token->attr.keyword == KEYWORD_PRINT)
                    return 7;
                else if(token->attr.keyword == KEYWORD_LENGTH)
                    return 8;
                else if(token->attr.keyword == KEYWORD_SUBSTR)
                    return 9;
                else if(token->attr.keyword == KEYWORD_ORD)
                    return 10;
                else if(token->attr.keyword == KEYWORD_CHR)
                    return 11;
                else if(token->attr.keyword == KEYWORD_INPUTS)
                    return 12;
                else if(token->attr.keyword == KEYWORD_INPUTI)
                    return 13;
                else if(token->attr.keyword == KEYWORD_INPUTF)
                    return 14;
            }
            else if(token->token == TYPE_IDENTIFIER)
                return 15;
            return 0;
            break;

        case DECLARE:
            if(token->token == TYPE_EOL)
                return 20;
            else if(token->token == TYPE_IDENTIFIER)
                return 16;
            //else if() // func... // TODO
            else if(token->token == TYPE_INT)
                return 17;
            else if(token->token == TYPE_FLOAT)
                return 17;
            else if(token->token == TYPE_STRING)
                return 17;
            break;

        case VALUE:
            if(token->token == TYPE_INT)
                return 29;
            else if(token->token == TYPE_STRING)
                return 30;
            else if(token->token == TYPE_STRING)
                return 31;
            return 0;
            break;

        case PARAMS:
            if(token->token == TYPE_IDENTIFIER)
                return 21;
            else if(token->token == TYPE_RIGHT_BRACKET)
                return 22;
            return 0;
            break;

        case PARAM:
            if(token->token == TYPE_COMMA)
                return 23;
            else if(token->token == TYPE_RIGHT_BRACKET)
                return 24;
            return 0;
            break;

        case ARGVS:
            if(token->token == TYPE_INT)
                return 29;
            else if(token->token == TYPE_FLOAT)
                return 30;
            else if(token->token == TYPE_STRING)
                return 31;
            return 0;
            break;

        case ARGV:
            if(token->token == TYPE_COMMA)
                return 27;
            else if(token->token == TYPE_RIGHT_BRACKET)
                return 28;
            return 0;
        
    }
        
}

void get_rule(int rule_num){
    switch (rule_num){
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        case 9:
            break;
        case 10:
            break;
        case 11:
            break;
        case 12:
            break;
        case 13:
            break;
        case 14:
            break;
        case 15:
            break;
    }
}