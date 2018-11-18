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

bool generator_start(); // vygeneruje hlavicku, preddefinovane funkce a jump na main
bool gen_mainscope_start(); // zacatek main
bool gen_mainscope_end(); // konec main


bool gen_func_start(char *func_id); // začátek funkce, dec 1
bool gen_func_end(char *func_id); // konec funkce, dec 5
bool gen_func_call(char *func_id); // volani funkce, call 3
bool gen_func_param(char *param_id, int idx); // vytvori promenou pro parametr s indexem idx, dec 2
bool gen_func_prep_for_params(); // generuje kod na pripravu predani parametru, call1
bool gen_func_pass_param(Token_t, int idx); // ulozeni parametru do promene, call2
bool gen_func_ret(char *func_id); // return, dec 4
bool gen_func_rval(); // deklarace navratove prom, dec 3
bool gen_func_rval_assign(char *var_id); // prirazeni do promene co vraci funkce, call 4
//bool gen_func_conv_param(Type_of_tHTItem one, Type_of_tHTItem two, int idx);// pretypuje param idx z typu one na typ two

bool gen_var_declar(char *var_id); // deklarace promenne
bool gen_var_defval(char *var_id); // inicalizuje prom na vychozi nil


bool gen_push(Token_t t); // vlozi token na zasobnik, prace s vyrazy
bool gen_input(char *var_id, Type_of_tHTItem t); // načtení do promene, t urcuje typ, napr.  x=inputi() tak gen_input("x", INTEGER);
bool gen_print(); // tiskne vysledek expression

// konverzni operace pro praci s vyrazy
bool gen_sop1_2flt();
bool gen_sop1_2int();
bool gen_sop2_2flt();
bool gen_sop2_2int();

bool gen_concat_str(); // konkatenace dvou promenych (string) na vrcholu
bool gen_save_expr_res(char *var_id, char *frame); // frame = "LF" - funkce, "GF" - main, veme promenou z vrcholu zasobniku a ulozi ji do promenne var
bool gen_stackop(Precedential_table_symbol symb); // provede operaci se dvema prom na vrcholu zasobniku, dle symbolu symb

// prace s if
bool gen_if_start(char *func_id, int l_idx, int deep);
bool gen_if_else(char *func_id, int l_idx, int deep);
bool gen_if_end(char *func_id, int l_idx, int deep);

// prace s while
bool gen_while_header(char *func_id, int l_idx, int deep);
bool gen_while_start(char *func_id, int l_idx, int deep);
bool gen_while_end(char *func_id, int l_idx, int deep);

static bool gen_builtin_funcs();

void clear_code(); // uvolni dyn. string s kodem
void flush_code(FILE *dst_file); // vyflusne kod na zadany vystup a uvolni dyn. string s kodem

#endif