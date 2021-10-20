#pragma once
#include "XString.h"

#define ENUM_TOKEN_TYPES \
	X(do) X(else) X(boolean) X(end) X(function) X(global) X(if) X(integer) X(local) X(nil) X(number) X(require) X(return) X(string) X(then) X(while)\
X(identifier)\
X(int_literal) X(double_literal) X(string_literal) X(true) X(false)\
X(length) X(multiply) X(divide) X(int_divide) X(add) X(subtract) X(concatenate) X(l) X(le) X(g) X(ge) X(e) X(ne) X(ee) X(modulo) X(power) X(not) X(and) X(or)\
X(left_parenthese) X(right_parenthese) X(comma) X(semicolumn)\
X(eof) X(err)

typedef enum token_type {
#define X(a) tt_##a,
	ENUM_TOKEN_TYPES
#undef X
}token_type;
typedef struct token token;

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

void print_tk(token* self);
