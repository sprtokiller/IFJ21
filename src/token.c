#include <stdlib.h>

#include "token.h"
#include "common.h"

const char* token_types_str[] =
{
#define X(a) #a,
	ENUM_TOKEN_TYPES
#undef X
};

//converts type_name enum to string
const char* token_type_name(token_type type) {
	switch (type) {
		SWITCH_TEXT(tt_int_literal, "integer literal");
		SWITCH_TEXT(tt_double_literal, "double literal");
		SWITCH_TEXT(tt_string_literal, "string literal");
		SWITCH_TEXT(tt_int_divide, "integer divide");
		SWITCH_TEXT(tt_l, "less then comparison");
		SWITCH_TEXT(tt_le, "less or equal then comparison");
		SWITCH_TEXT(tt_g, "greater then comparison");
		SWITCH_TEXT(tt_ge, "greater or equal then comparison");
		SWITCH_TEXT(tt_assign, "assignment");
		SWITCH_TEXT(tt_ne, "not equality");
		SWITCH_TEXT(tt_ee, "equality");
		SWITCH_TEXT(tt_not, "logical not");
		SWITCH_TEXT(tt_and, "logical and");
		SWITCH_TEXT(tt_or, "logical or");
		SWITCH_TEXT(tt_left_parenthese, "left parenthese");
		SWITCH_TEXT(tt_right_parenthese, "right parenthese");
		SWITCH_TEXT(tt_err, RED "Err" RESET);
	default:
		return token_types_str[type];
	}
}

void token_exp_ctor(token* self, void* expression)
{
	self->line = 0;
	self->column = 0;
	self->var = v_expr;
	self->type = tt_expression;
	self->expression = expression;
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

extern inline void token_move_ctor(token* self, token* other);
extern inline bool is_type(token_type tt);
extern inline token_type to_type(token_type tt);
extern inline token_type literal_type(token_type tt);

void print_tk(token* self)
{
	d_msg("Token at [%d, %d]: %s", self->line, self->column, token_type_name(self->type));
	switch (self->var)
	{
	case v_int:
		d_msg(" Val: %d", self->ival);
		break;
	case v_flt:
		d_msg(" Val: %.9lf", self->dval);
		break;
	case v_str:
		d_msg(" Val: %s", c_str(&self->sval));
		break;
	default:
		break;
	}
}
