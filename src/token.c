#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "token.h"
#include "common.h"

#define SWITCH_TEXT(tt, str) case tt: return str

const char* tk_group_str[tkg_size] =
{
#define X(a) #a,
ENUM_GROUPS
#undef X
};

const char* group_name(enum token_group group)
{
	return tk_group_str[group];
}

//allocates token and inicializes it
token_s* init_t(char* val, uint32_t line, uint32_t col, token_type type, token_group_t group) {
	token_s* t = malloc(sizeof(token_s));
	if (!t) {
		e_msg("Alocation failed.\n");
		return NULL;
	}
	// TODO: add define to disable debug info for faster scanner
	t->warning = NULL;
	t->line = line;
	t->column = col;
	t->type = type;
	t->group = group;
	t->val = val;
	return t;
}

//converts type_name enum to string
const char* type_name(token_type type) {
	switch (type) {
		SWITCH_TEXT(tt_do, "do");
		SWITCH_TEXT(tt_else, "else");
		SWITCH_TEXT(tt_boolean, "boolean");
		SWITCH_TEXT(tt_end, "end");
		SWITCH_TEXT(tt_function, "function");
		SWITCH_TEXT(tt_global, "global");
		SWITCH_TEXT(tt_if, "if");
		SWITCH_TEXT(tt_integer, "integer");
		SWITCH_TEXT(tt_local, "local");
		SWITCH_TEXT(tt_nil, "nil");
		SWITCH_TEXT(tt_number, "number");
		SWITCH_TEXT(tt_require, "require");
		SWITCH_TEXT(tt_return, "return");
		SWITCH_TEXT(tt_string, "string");
		SWITCH_TEXT(tt_then, "then");
		SWITCH_TEXT(tt_while, "while");
		SWITCH_TEXT(tt_identifier, "identifier");
		SWITCH_TEXT(tt_int_literal, "integer");
		SWITCH_TEXT(tt_double_literal, "double");
		SWITCH_TEXT(tt_string_literal, "string");
		SWITCH_TEXT(tt_true, "true");
		SWITCH_TEXT(tt_false, "false");
		SWITCH_TEXT(tt_length, "length");
		SWITCH_TEXT(tt_multiply, "multiply");
		SWITCH_TEXT(tt_divide, "divide");
		SWITCH_TEXT(tt_int_divide, "integer divide");
		SWITCH_TEXT(tt_add, "add");
		SWITCH_TEXT(tt_subtract, "subtract");
		SWITCH_TEXT(tt_concatenate, "concatenate");
		SWITCH_TEXT(tt_l, "less then comparison");
		SWITCH_TEXT(tt_le, "less or equal then comparison");
		SWITCH_TEXT(tt_g, "greater then comparison");
		SWITCH_TEXT(tt_ge, "greater or equal then comparison");
		SWITCH_TEXT(tt_e, "assignment");
		SWITCH_TEXT(tt_ne, "not equality");
		SWITCH_TEXT(tt_ee, "equality");
		SWITCH_TEXT(tt_modulo, "modulo");
		SWITCH_TEXT(tt_power, "power");
		SWITCH_TEXT(tt_not, "logical not");
		SWITCH_TEXT(tt_and, "logical and");
		SWITCH_TEXT(tt_or, "logical or");
		SWITCH_TEXT(tt_left_parenthese, "left parenthese");
		SWITCH_TEXT(tt_right_parenthese, "right parenthese");
		SWITCH_TEXT(tt_comma, "comma");
		SWITCH_TEXT(tt_eof, "EOF");
	default:
		return "error";
	}
}

// prints token information
void print_t(token_s* t) {
	d_msg("Token at [%d, %d]: %s %s", t->line, t->column, type_name(t->type), group_name(t->group));

	if (t->val)
		d_msg("Val:     %s", t->val);

	if (t->warning)
		w_msg("%s", t->warning);
}

void token_ctor(token* self, token_type ty, String* val)
{
	self->line = 0;
	self->column = 0;
	self->type = ty;
	self->var = v_none;
	char* endptr = NULL;
	if (ty == tt_int_literal){
		self->var = v_int;
		self->ival = strtoll(c_str(val), &endptr, 10);
		clear_str(val);
	}else if (ty == tt_double_literal){
		self->var = v_flt;
		self->dval = strtod(c_str(val), &endptr);
		clear_str(val);
	}else if (val) {
		self->var = v_str;
		String_move_ctor(&self->sval, val);
	}
}

void token_dtor(token* self)
{
	if (self->var == v_str)
		String_dtor(&self->sval);
}

void print_tk(token* self)
{
	d_msg("Token at [%d, %d]: %s", self->line, self->column, type_name(self->type));
	switch (self->var)
	{
	case v_int:
		d_msg("Val: %d", self->ival);
		break;
	case v_flt:
		d_msg("Val: %.9lf", self->dval);
		break;
	case v_str:
		d_msg("Val: %s", self->sval);
		break;
	default:
		break;
	}
}
