#pragma once
#include "XString.h"

#define ENUM_TOKEN_TYPES \
X(err) X(do) X(else) X(elseif) X(boolean) X(end) X(function) X(global) X(if) X(integer) X(local) X(nil) X(number) X(require) X(return) X(string) X(then) X(while)\
X(identifier)\
X(int_literal) X(double_literal) X(string_literal) X(true) X(false)\
X(length) X(multiply) X(divide) X(int_divide) X(add) X(subtract) X(concatenate) X(l) X(le) X(g) X(ge) X(ee) X(ne) X(modulo) X(power) X(not) X(and) X(or)\
X(left_parenthese) X(right_parenthese) X(comma) X(semicolon) X(colon) X(assign)\
X(eof) X(u_plus) X(u_minus) X(fcall)

typedef enum token_type {
#define X(a) tt_##a,
	ENUM_TOKEN_TYPES
#undef X
	T0, T68 = T0 + 68
}token_type;

const char* token_type_name(token_type type);

typedef struct token token;

typedef struct
{
	//the data we actually care about
	token_type type : sizeof(token_type) * 8 - 2;

	uint8_t var : 2;
	union
	{
		String sval;
		double dval;
		int64_t ival;
	};
}token_core;


enum val_ty { v_none, v_str, v_flt, v_int };

struct token {
	//for debugging
	uint32_t line;
	uint32_t column;

	//the data we actually care about
	token_type type : sizeof(token_type) * 8 - 2;

	uint8_t var : 2;
	union
	{
		String sval;
		double dval;
		int64_t ival;
	};
};

void token_ctor(token* self, token_type ty, String* val);
void token_dtor(token* self);
inline void token_move_ctor(token* self, token* other)
{
	*self = *other;
	other->var = v_none;
}

void print_tk(token* self);
