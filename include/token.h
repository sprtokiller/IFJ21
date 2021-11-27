#pragma once
#include "XString.h"
#include <stdint.h>

#define ENUM_TOKEN_TYPES \
X(err) X(do) X(else) X(elseif) X(boolean) X(integer) X(number) X(string) X(end) X(function) X(global) X(if)  X(local) X(nil)  X(require) X(return)  X(then) X(while)\
X(identifier)\
X(int_literal) X(double_literal) X(string_literal) X(true) X(false)\
X(length) X(multiply) X(divide) X(int_divide) X(add) X(subtract) X(concatenate) X(l) X(le) X(g) X(ge) X(ee) X(ne) X(modulo) X(power) X(not) X(and) X(or)\
X(left_parenthese) X(right_parenthese) X(comma) X(semicolon) X(colon) X(assign)\
X(eof) X(u_plus) X(u_minus) X(fcall) X(type)\
X(for) X(repeat) X(until)\
X(expression)

typedef enum token_type {
#define X(a) tt_##a,
	ENUM_TOKEN_TYPES
#undef X
	T0, T73 = T0 + 73
}token_type;

const char* token_type_name(token_type type);

typedef struct token token;

typedef struct
{
	//the data we actually care about
	token_type type : sizeof(token_type) * 8 - 3;

	uint8_t var : 3;
	union
	{
		String sval;
		double dval;
		int64_t ival;
		struct
		{
			void* expression;
			int ec;
		};
	};
}token_core;


enum val_ty { v_none, v_str, v_flt, v_int, v_bool, v_expr };

struct token {
	//for debugging
	uint32_t line;
	uint32_t column;

	//the data we actually care about
	token_type type : sizeof(token_type) * 8 - 3;

	uint8_t var : 3;
	union
	{
		String sval;
		double dval;
		int64_t ival;
		struct
		{
			void* expression;
			int ec;
		};
	};
};

void token_exp_ctor(token* self, void* expression);
void token_ctor(token* self, token_type ty, String* val);
void token_dtor(token* self);

inline void token_move_ctor(token* self, token* other)
{
	*self = *other;
	other->var = v_none;
}
inline bool is_type(token_type tt)
{
	return tt >= tt_boolean && tt <= tt_string;
}
inline token_type to_type(token_type tt)
{
	return tt >= tt_boolean && tt <= tt_string?tt_type:tt;
}

void print_tk(token* self);
