/*
IFJ 2018
Adam Hostin, xhosti02
Sabína Gregušová, xgregu02
Dominik Peza, xpezad00
Adrián Tulušák, xtulus00
*/

#include "lexer.h"
#include "instructions.h"
#include "dynamic_string.h"
#include <ctype.h>

// todo vestavene funkce typ kontrola + navrat

#define ADD_INST(_inst)		\
	if (!add_const_string(&code, (_inst "\n"))) return false 

#define ADD_CODE(_code)		\
	if (!add_const_string(&code, (_code))) return false

#define ADD_INT(_code)				\
	do {							\
		char num[41];				\
		sprintf(num, "%d", _code);	\
		ADD_CODE(num);				\
	} while (0) 					

#define FUNC_LEN				\
	"\n LABEL &length"			\
	"\n PUSHFRAME"				\
	"\n DEFVAR LF@_rval"		\
	"\n STRLEN LF@_rval LF@_0"	\
	"\n POPFRAME"				\
	"\n RETURN"					

#define FUNC_SUBS													\
	"\n LABEL &substr"												\
	"\n PUSHFRAME"													\
	"\n DEFVAR LF@_rval"											\
	"\n MOVE LF@_rval string@"										\
	"\n DEFVAR LF@str_len"											\
	"\n CREATEFRAME"												\
	"\n DEFVAR TF@_0"												\
	"\n MOVE TF@_0 LF@_0"											\
	"\n CALL &length"												\
	"\n MOVE LF@str_len TF@_rval"									\
	"\n DEFVAR LF@ret_cond"											\
	"\n LT LF@ret_cond LF@str_len int@0"							\
	"\n JUMPIFEQ &substr&return LF@ret_cond bool@true"				\
	"\n EQ LF@ret_cond LF@str_len int@0"							\
	"\n JUMPIFEQ &substr&return LF@ret_cond bool@true"				\
	"\n LT LF@ret_cond LF@_1 int@0"									\
	"\n JUMPIFEQ &substr&return LF@ret_cond bool@true"				\
	"\n EQ LF@ret_cond LF@_1 int@0"									\
	"\n JUMPIFEQ &substr&return LF@ret_cond bool@true"				\
	"\n GT LF@ret_cond LF@_1 LF@str_len"							\
	"\n JUMPIFEQ &substr&return LF@ret_cond bool@true"				\
	"\n EQ LF@ret_cond LF@_2 int@0"									\
	"\n JUMPIFEQ &substr&return LF@ret_cond bool@true"				\
	"\n DEFVAR LF@max_n"											\
	"\n MOVE LF@max_n LF@str_len"									\
	"\n SUB LF@max_n LF@max_n LF@_1"								\
	"\n ADD LF@max_n LF@max_n int@1"								\
	"\n DEFVAR LF@edit_n_cond"										\
	"\n LT LF@edit_n_cond LF@_2 int@0"								\
	"\n JUMPIFEQ &substr&edit_n LF@edit_n_cond bool@true"			\
	"\n GT LF@edit_n_cond LF@_2 LF@max_n"							\
	"\n JUMPIFEQ &substr&edit_n LF@edit_n_cond bool@true"			\
	"\n JUMP &substr&process"										\
	"\n LABEL &substr&edit_n"										\
	"\n MOVE LF@_2 LF@max_n"										\
	"\n LABEL &substr&process"										\
	"\n DEFVAR LF@index"											\
	"\n MOVE LF@index LF@_1"										\
	"\n SUB LF@index LF@index int@1"								\
	"\n DEFVAR LF@char"												\
	"\n DEFVAR LF@process_loop_cond"								\
	"\n LABEL &substr&process_loop"									\
	"\n GETCHAR LF@char LF@_0 LF@index"								\
	"\n CONCAT LF@_rval LF@_rval LF@char"							\
	"\n ADD LF@index LF@index int@1"								\
	"\n SUB LF@_2 LF@_2 int@1"										\
	"\n GT LF@process_loop_cond LF@_2 int@0"						\
	"\n JUMPIFEQ &substr&process_loop LF@process_loop_cond bool@true" \
	"\n LABEL &substr&return"										\
	"\n POPFRAME"													\
	"\n RETURN"														

#define FUNC_ORD											\
	"\n LABEL &ord"											\
	"\n PUSHFRAME"											\
	"\n DEFVAR LF@_rval"									\
	"\n MOVE LF@_rval int@0"								\
	"\n DEFVAR LF@cond_length"								\
	"\n LT LF@cond_length LF@_1 int@1"						\
	"\n JUMPIFEQ &ord&return LF@cond_length bool@true"		\
	"\n DEFVAR LF@str_len"									\
	"\n CREATEFRAME"										\
	"\n DEFVAR TF@_0"										\
	"\n MOVE TF@_0 LF@_0"									\
	"\n CALL &length"										\
	"\n MOVE LF@str_len TF@_rval"							\
	"\n GT LF@cond_length LF@_1 LF@str_len"					\
	"\n JUMPIFEQ &ord&return LF@cond_length bool@true"		\
	"\n SUB LF@_1 LF@_1 int@1"								\
	"\n STRI2INT LF@_rval LF@_0 LF@_1"						\
	"\n LABEL &ord&return"									\
	"\n POPFRAME"											\
	"\n RETURN"												

#define FUNC_CHR										\
	"\n LABEL &chr"										\
	"\n PUSHFRAME"										\
	"\n DEFVAR LF@_rval"								\
	"\n MOVE LF@_rval string@"							\
	"\n DEFVAR LF@cond_range"							\
	"\n LT LF@cond_range LF@_0 int@0"					\
	"\n JUMPIFEQ &chr&return LF@cond_range bool@true"	\
	"\n GT LF@cond_range LF@_0 int@255"					\
	"\n JUMPIFEQ &chr&return LF@cond_range bool@true"	\
	"\n INT2CHAR LF@_rval LF@_0"						\
	"\n LABEL &chr&return"								\
	"\n POPFRAME"										\
	"\n RETURN"


string_t code;

int auxcat = 1;

static bool generate_header()
{
	ADD_INST(".IFJcode18");
	ADD_INST("DEFVAR GF@_exp_res");
	ADD_INST("DEFVAR GF@_aux_1");
	ADD_INST("DEFVAR GF@_aux_2");
	ADD_INST("DEFVAR GF@_aux_3");


	ADD_INST("DEFVAR GF@_exp_res_type");
	ADD_INST("DEFVAR GF@_aux_1_type");
	ADD_INST("DEFVAR GF@_aux_2_type");
	ADD_INST("DEFVAR GF@_aux_3_type");

	ADD_INST("JUMP &&main");
	return true;
}

static bool gen_type_check()
{
	ADD_INST("LABEL &type_check");
	ADD_INST("PUSHFRAME");
	ADD_INST("POPS GF@_aux_1");
	ADD_INST("POPS GF@_aux_2");


	ADD_INST("TYPE GF@_aux_1_type GF@_aux_1");
	ADD_INST("TYPE GF@_aux_2_type GF@_aux_2");

	ADD_INST("JUMPIFEQ &must_be_same GF@_aux_1_type string@string");

	ADD_INST("JUMPIFEQ &one_is_int GF@_aux_1_type string@int");
	ADD_INST("JUMPIFEQ &one_is_flt GF@_aux_1_type string@float");

	ADD_INST("LABEL &one_is_int");
	ADD_INST("JUMPIFEQ &must_be_same GF@_aux_2_type string@int");
	ADD_INST("JUMPIFEQ &must_be_same GF@_aux_2_type string@string");
	ADD_INST("INT2FLOAT GF@_aux_1 GF@_aux_1");
	ADD_INST("JUMP &type_check_true");

	
	ADD_INST("LABEL &one_is_flt");
	ADD_INST("JUMPIFEQ &must_be_same GF@_aux_2_type string@float");
	ADD_INST("JUMPIFEQ &must_be_same GF@_aux_2_type string@string");
	ADD_INST("INT2FLOAT GF@_aux_2 GF@_aux_2");
	ADD_INST("JUMP &type_check_true");

	ADD_INST("LABEL &must_be_same");
	ADD_INST("JUMPIFEQ &type_check_true GF@_aux_1_type GF@_aux_2_type");

	ADD_INST("EXIT int@4");

	ADD_INST("LABEL &type_check_true");

	ADD_INST("PUSHS GF@_aux_2");
	ADD_INST("PUSHS GF@_aux_1");

	ADD_INST("POPFRAME");
	ADD_INST("RETURN");

	return true;

}

static bool gen_builtin_funcs()
{
	ADD_INST(FUNC_LEN);
	ADD_INST(FUNC_SUBS);
	ADD_INST(FUNC_ORD);
	ADD_INST(FUNC_CHR);

	return true;
}

bool generator_start()
{
	if (!allocate_string(&code)) return false;

	if (!generate_header()) return false;

	if (!gen_builtin_funcs()) return false;

	if (!gen_type_check()) return false;

	return true;
}



bool gen_mainscope_start()
{
	ADD_INST("LABEL &&main");
	ADD_INST("CREATEFRAME");
	ADD_INST("PUSHFRAME");

	return true;
}

bool gen_mainscope_end()
{
	ADD_INST("POPFRAME");
	ADD_INST("CLEARS");

	return true;
}


void clear_code()
{
	free_string(&code);
}

void flush_code(FILE *dst_file)
{
	fputs(code.s, dst_file);
	clear_code();
}

bool gen_func_start(char *func_id)
{

	ADD_CODE("JUMP &endof_");
	ADD_CODE(func_id);
	ADD_CODE("\n");
	ADD_CODE("LABEL &");
	ADD_CODE(func_id);
	ADD_CODE("\n");
	ADD_INST("PUSHFRAME");
	ADD_INST("MOVE GF@_exp_res nil@nil");
	return true;
}


bool gen_func_end(char *func_id)
{

	ADD_CODE("LABEL &");
	ADD_CODE(func_id);
	ADD_CODE("_return\n");
	ADD_INST("POPFRAME");
	ADD_INST("RETURN");
	ADD_CODE("LABEL &endof_");
	ADD_CODE(func_id);
	ADD_CODE("\n");
	return true;
}

bool gen_func_call(char *func_id)
{
	ADD_CODE("CALL &");
	ADD_CODE(func_id);
	ADD_CODE("\n");
	return true;
}

bool gen_func_param(char *param_id, int idx)
{
	ADD_CODE("DEFVAR LF@");
	ADD_CODE(param_id);
	ADD_CODE("\n");
	ADD_CODE("MOVE LF@");
	ADD_CODE(param_id);
	ADD_CODE(" LF@_");
	ADD_INT(idx);
	ADD_CODE("\n");

	ADD_CODE("DEFVAR LF@");
	ADD_CODE(param_id);
	ADD_CODE("_type\n");
	ADD_CODE("TYPE LF@");
	ADD_CODE(param_id);
	ADD_CODE("_type LF@");
	ADD_CODE(param_id);
	ADD_CODE("\n");


	return true;
}

bool gen_func_prep_for_params()
{
	ADD_INST("CREATEFRAME");
	return true;
}

static bool gen_def_varval()
{
	ADD_CODE("nil@nil");

	return true;
}


static bool gen_term_val(Tmp_Token_t t)
{
	unsigned char c;
	char t_str[41];
	
	string_t tmp_str;
	if (!allocate_string(&tmp_str))
		return false;

	switch (t.type_token)
	{
		case TYPE_INT:
			sprintf(t_str, "%d", t.attr_token.tmp_integer);
			ADD_CODE("int@"); 
			ADD_CODE(t_str);
			break;

		case TYPE_FLOAT:
			sprintf(t_str, "%a", t.attr_token.tmp_flt);
			ADD_CODE("float@");
			ADD_CODE(t_str);
			break;

		case TYPE_STRING:
			for (int i = 0; (c = (unsigned char) (t.attr_token.tmp_string)[i]) != '\0'; i++)
			{
				if (c == '\\' || c == '#' || c <= 32 || !isprint(c))
				{
					add_char(&tmp_str, '\\');
					sprintf(t_str, "%03d", c);
					add_const_string(&tmp_str, t_str);
				}
				else
				{
					add_char(&tmp_str, c);
				}
			}
			ADD_CODE("string@");
			ADD_CODE(tmp_str.s);
			break;

		case TYPE_IDENTIFIER:
			ADD_CODE("LF@");
			ADD_CODE(t.attr_token.tmp_string);
			break;

		default:
			free_string(&tmp_str);
			return false;
	}

	free_string(&tmp_str);
	return true;
}

static bool gen_term_val_classic(Token_t t)
{
	unsigned char c;
	char t_str[41];
	
	string_t tmp_str;
	if (!allocate_string(&tmp_str))
		return false;

	switch (t.token)
	{
		case TYPE_INT:
			sprintf(t_str, "%d", t.attr.integer);
			ADD_CODE("int@"); 
			ADD_CODE(t_str);
			break;

		case TYPE_FLOAT:
			sprintf(t_str, "%a", t.attr.flt);
			ADD_CODE("float@");
			ADD_CODE(t_str);
			break;

		case TYPE_STRING:
			for (int i = 0; (c = (unsigned char) (t.attr.string->s)[i]) != '\0'; i++)
			{
				if (c == '\\' || c == '#' || c <= 32 || !isprint(c))
				{
					add_char(&tmp_str, '\\');
					sprintf(t_str, "%03d", c);
					add_const_string(&tmp_str, t_str);
				}
				else
				{
					add_char(&tmp_str, c);
				}
			}
			ADD_CODE("string@");
			ADD_CODE(tmp_str.s);
			break;

		case TYPE_IDENTIFIER:
			ADD_CODE("LF@");
			ADD_CODE(t.attr.string->s);
			break;

		default:
			free_string(&tmp_str);
			return false;
	}

	free_string(&tmp_str);
	return true;
}

bool gen_func_pass_param(Token_t t, int idx)
{
	ADD_CODE("DEFVAR TF@_");
	ADD_INT(idx);
	ADD_CODE("\n");
	ADD_CODE("MOVE TF@_");
	ADD_INT(idx);
	ADD_CODE(" ");
	if (!gen_term_val_classic(t)) 
		return false; 
	ADD_CODE("\n");


	return true;
}


bool gen_func_ret(char *func_id)
{
	ADD_INST("MOVE LF@_rval GF@_exp_res");
	ADD_CODE("JUMP &");
	ADD_CODE(func_id);
	ADD_CODE("_return\n");
	return true;
}



bool gen_var_declar(char *var_id)
{
	ADD_CODE("DEFVAR LF@");
	ADD_CODE(var_id);
	ADD_CODE("\n");

	ADD_CODE("DEFVAR LF@");
	ADD_CODE(var_id);
	ADD_CODE("_type\n");

	ADD_CODE("MOVE LF@");
	ADD_CODE(var_id);
	ADD_CODE("_type string@nil\n");
	return true;
}


bool gen_var_defval(char *var_id)
{
	ADD_CODE("MOVE LF@");
	ADD_CODE(var_id);
	ADD_CODE(" ");
	if (!gen_def_varval())
		return false; 
	ADD_CODE("\n");
	return true;
}

bool gen_push(Tmp_Token_t t)
{
	ADD_CODE("PUSHS ");
	if (!gen_term_val(t))
	 return false;
	ADD_CODE("\n");
	return true;
}


bool gen_stackop(Precedential_table_symbol symb) // rules?
{

	auxcat++;
	switch (symb)
	{
		case PLUS:
			ADD_INST("CREATEFRAME");
			ADD_INST("CALL &type_check");
			ADD_CODE("JUMPIFEQ &cat");


			ADD_INT(auxcat);
			ADD_CODE("GF@_aux_1_type string@string")
			ADD_CODE("\n");

			ADD_CODE("JUMP &adds");

			ADD_CODE("LABEL &cat");

			ADD_INT(auxcat);
			ADD_CODE("\n");

			ADD_INST("POPS GF@_aux_1");
			ADD_INST("POPS GF@_aux_2");
			ADD_INST("CONCAT GF@_aux_1 GF@_aux_1 GF@_aux_2");
			ADD_INST("PUSHS GF@_aux_1");

			ADD_CODE("JUMP &notadds");


			ADD_CODE("LABEL &adds");

			ADD_INT(auxcat);
			ADD_CODE("\n");


			ADD_INST("ADDS");

			ADD_CODE("LABEL &notadds");

			ADD_INT(auxcat);
			ADD_CODE("\n");
			break;

		case MINUS:
			ADD_INST("CREATEFRAME");
			ADD_INST("CALL &type_check");
			ADD_INST("SUBS");
			break;

		case MUL:
			ADD_INST("CREATEFRAME");
			ADD_INST("CALL &type_check");
			ADD_INST("MULS");
			break;

		case DIV:
			ADD_INST("CREATEFRAME");
			ADD_INST("CALL &type_check");
			ADD_INST("DIVS");
			break;

	/*	case IDIV:
			ADD_INST("POPS GF@_aux_1");
			ADD_INST("INT2FLOATS");
			ADD_INST("PUSHS GF@_aux_1");
			ADD_INST("INT2FLOATS");
			ADD_INST("DIVS");
			ADD_INST("FLOAT2INTS");
			break; */

		case EQL:
			ADD_INST("CREATEFRAME");
			ADD_INST("CALL &type_check");
			ADD_INST("EQS");
			break;

		case NEQ:
			ADD_INST("CREATEFRAME");
			ADD_INST("CALL &type_check");
			ADD_INST("EQS");
			ADD_INST("NOTS");
			break;

		case LEQ:
			ADD_INST("CREATEFRAME");
			ADD_INST("CALL &type_check");
			ADD_INST("POPS GF@_aux_1");
			ADD_INST("POPS GF@_aux_2");
			ADD_INST("PUSHS GF@_aux_2");
			ADD_INST("PUSHS GF@_aux_1");
			ADD_INST("LTS");
			ADD_INST("PUSHS GF@_aux_2");
			ADD_INST("PUSHS GF@_aux_1");
			ADD_INST("EQS");
			ADD_INST("ORS");
			break;

		case LTN:
			ADD_INST("CREATEFRAME");
			ADD_INST("CALL &type_check");
			ADD_INST("LTS");
			break;

		case MEQ:
			ADD_INST("CREATEFRAME");
			ADD_INST("CALL &type_check");
			ADD_INST("POPS GF@_aux_1");
			ADD_INST("POPS GF@_aux_2");
			ADD_INST("PUSHS GF@_aux_2");
			ADD_INST("PUSHS GF@_aux_1");
			ADD_INST("GTS");
			ADD_INST("PUSHS GF@_aux_2");
			ADD_INST("PUSHS GF@_aux_1");
			ADD_INST("EQS");
			ADD_INST("ORS");
			break;

		case MTN:
			ADD_INST("CREATEFRAME");
			ADD_INST("CALL &type_check");
			ADD_INST("GTS");
			break;

		default:
			break;
	}

	return true;
}


bool generate_concat_stack_strings()
{
	ADD_INST("POPS GF@_aux_3");
	ADD_INST("POPS GF@_aux_2");
	ADD_INST("CONCAT GF@_aux_1 GF@_aux_2 GF@_aux_3");
	ADD_INST("PUSHS GF@_aux_1");

	return true;
}


bool gen_input(char *var_id, Type_of_tHTItem t, bool assign)
{
	// if assign + i do exp result
	ADD_CODE("READ LF@");
	ADD_CODE(var_id);
	ADD_CODE(" ");

	switch (t)
	{
		case INTEGER:
			ADD_CODE("int");
			break;

		case PRASATKO_S_PAPUCKAMI_FLT:
			ADD_CODE("float");
			break;

		case STRING:
			ADD_CODE("string");
			break;

		default:
			return false;
	}
	ADD_CODE("\n");
	return true;
}

bool gen_print()
{
	ADD_INST("WRITE GF@_exp_res");
	return true;
}

bool gen_sop1_2flt()
{
	ADD_INST("INT2FLOATS");
	return true;
}


bool gen_sop1_2int()
{
	ADD_INST("FLOAT2INTS");
	return true;
}


bool gen_sop2_2flt()
{
	ADD_INST("POPS GF@_aux_1");
	ADD_INST("INT2FLOATS");
	ADD_INST("PUSHS GF@_aux_1");
	return true;
}


bool gen_sop2_2int()
{
	ADD_INST("POPS GF@_aux_1");
	ADD_INST("FLOAT2INTS");
	ADD_INST("PUSHS GF@_aux_1");
	return true;
}


static bool gen_label(int l_idx, int deep)
{
	ADD_CODE("LABEL &");

	ADD_INT(deep);
	ADD_CODE("_");
	ADD_INT(l_idx);
	ADD_CODE("\n");
	return true;
}


bool gen_if_start(int l_idx, int deep)
{
	ADD_CODE("JUMPIFEQ &");

	ADD_INT(deep);
	ADD_CODE("_");
	ADD_INT(l_idx);
	ADD_CODE(" GF@_exp_res bool@false\n");
	return true;
}


bool gen_if_else(int l_idx, int deep)
{
	ADD_CODE("JUMP &");

	ADD_INT(deep);
	ADD_CODE("_");
	ADD_INT(l_idx + 1);
	ADD_CODE("\n");
	if (!gen_label(l_idx, deep))
		return false;
	return true;
}


bool gen_if_end(int l_idx, int deep)
{
	if (!gen_label(l_idx, deep))
		return false;
	return true;
}


bool gen_while_header(int l_idx, int deep)
{
	if (!gen_label(l_idx, deep))
		return false;
	return true;
}


bool gen_while_start(int l_idx, int deep)
{
	ADD_CODE("JUMPIFEQ &");

	ADD_INT(deep);
	ADD_CODE("_");
	ADD_INT(l_idx);
	ADD_CODE(" GF@_exp_res bool@false");
	ADD_CODE("\n");
	return true;
}


bool gen_while_end(int l_idx, int deep)
{
	ADD_CODE("JUMP &");

	ADD_INT(deep);
	ADD_CODE("_");
	ADD_INT(l_idx - 1);
	ADD_CODE("\n");
	if (!gen_label(l_idx, deep))
		return false;
	return true;
}

bool gen_func_rval()
{
	ADD_INST("DEFVAR LF@_rval");

	return true;
}

bool gen_func_rval_assign(char *var_id)
{


	ADD_CODE("MOVE LF@");
	ADD_CODE(var_id);
	ADD_CODE(" TF@_rval\n");

	ADD_CODE("TYPE LF@");
	ADD_CODE(var_id);
	ADD_CODE("_type LF@");
	ADD_CODE(var_id);
	ADD_CODE("\n");

	return true;
}

/*
bool gen_func_conv_param(Type_of_tHTItem one, Type_of_tHTItem two, int idx)
{
	if (one == PRASATKO_S_PAPUCKAMI_FLT && two == INTEGER)
	{
		ADD_CODE("FLOAT2INT TF@_");
		ADD_INT(idx);
		ADD_CODE(" TF@_");
		ADD_INT(idx);
		ADD_CODE("\n");
	}
	else if (one == INTEGER && two == PRASATKO_S_PAPUCKAMI_FLT)
	{
		ADD_CODE("INT2FLOAT TF@_");
		ADD_INT(idx);
		ADD_CODE(" TF@_");
		ADD_INT(idx);
		ADD_CODE("\n");
	}
	return true;
} */

bool gen_concat_str() // konkatenace todo
{
	ADD_INST("POPS GF@_aux_3");
	ADD_INST("POPS GF@_aux_2");
	ADD_INST("CONCAT GF@_aux_1 GF@_aux_2 GF@_aux_3");
	ADD_INST("PUSHS GF@_aux_1");
	return true;
}


bool gen_assign(char *var_id)
{


	ADD_CODE("MOVE LF");
	ADD_CODE("@");
	ADD_CODE(var_id);
	ADD_CODE(" GF@_exp_res");
	ADD_CODE("\n");


	return true;
}


bool gen_save_expr_res()
{

	ADD_INST("POPS GF@_exp_res");

	return true;
}

