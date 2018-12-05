/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "symtable.h"
#include "expression.h"
#include "parser.h"
#include "lexer.h"

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H


// functions for generating mainscope structure
bool generator_start(); 
bool gen_mainscope_start(); 
bool gen_mainscope_end();

// function for generating new dynamic string code
bool gen_new_part();

// function for generating definition of variable in if or while
bool gen_defvar_2_old(char *var_id);

// functions for generating function definition and calling instructions
bool gen_func_start(char *func_id); 
bool gen_func_end(char *func_id); 
bool gen_func_call(char *func_id);
bool gen_func_param(char *param_id, int idx); 
bool gen_func_prep_for_params(); 
bool gen_func_pass_param(Token_t, int idx); 
bool gen_func_ret(char *func_id); 
bool gen_func_rval(); 
bool gen_func_rval_assign(char *var_id); 

// functions for work with variables
bool gen_var_declar(char *var_id); 
bool gen_var_defval(char *var_id); 
bool gen_assign(char *var_id); 
// funstions for work with while cycle
// functions for work with expressions and i/o functions
bool gen_push(Tmp_Token_t t); 
bool gen_input(char *var_id, Type_of_tHTItem t, bool assign); 
bool gen_print();
bool gen_save_expr_res(); 
bool gen_stackop(Precedential_table_symbol symb); 

// functions for work with if statements
bool gen_if_start(int l_idx, int deep);
bool gen_if_else(int l_idx, int deep);
bool gen_if_end(int l_idx, int deep);

// funstions for work with while cycle
bool gen_while_header(int l_idx, int deep);
bool gen_while_start(int l_idx, int deep);
bool gen_while_end(int l_idx, int deep);

// functions for clearing and printig dynamic strings
void clear_code(); 
void flush_code(FILE *dst_file); 

#endif