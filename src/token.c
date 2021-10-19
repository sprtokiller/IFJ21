#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "token.h"
#include "common.h"

#define SWITCH_TEXT(tt, str) case tt: return str

//allocates token and inicializes it
token_s* init_t(char* val, uint line, uint col, token_type_t type, token_group_t group) {
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
const char* group_name(token_group_t group) {
	switch (group) {
		SWITCH_TEXT(tg_keyword, "keyword");
		SWITCH_TEXT(tg_identifier, "identifier");
		SWITCH_TEXT(tg_literal, "literal");
		SWITCH_TEXT(tg_operator, "operator");
		SWITCH_TEXT(tg_special, "symbol");
		SWITCH_TEXT(tg_eof, "EOF");
	default:
		return "error";
	}
}

//converts type_name enum to string
const char* type_name(token_type_t type) {
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
