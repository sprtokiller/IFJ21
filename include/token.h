#pragma once
#include "common.h"

//don't use this!
//just for editors autocomplete
enum token_group { tg_keyword, tg_identifier, tg_literal, tg_operator, tg_special, tg_eof, tg_err };

//don't use this!
//just for editors autocomplete
enum token_type {
	tt_do, tt_else, tt_boolean, tt_end, tt_function, tt_global, tt_if, tt_integer, tt_local, tt_nil, tt_number, tt_require, tt_return, tt_string, tt_then, tt_while,
	tt_identifier,
	tt_int_literal, tt_double_literal, tt_string_literal, tt_true, tt_false,
	tt_length, tt_multiply, tt_divide, tt_int_divide, tt_add, tt_subtract, tt_concatenate, tt_l, tt_le, tt_g, tt_ge, tt_e, tt_ne, tt_ee, tt_modulo, tt_power, tt_not, tt_and, tt_or,
	tt_left_parenthese, tt_right_parenthese, tt_comma, tt_semicolumn,
	tt_eof, tt_err
};

//enum type of token_group_t
typedef enum token_group token_group_t;

//enum type of token_group_t
typedef enum token_type token_type_t;

//dont use this!
//just for editors autocomplete
struct token_struct {
	//for debugging
	uint line;
	uint column;
	char* warning;

	//the data we actually care about
	token_group_t group;
	token_type_t type;
	char* val;
};

//stuct of token
typedef struct token_struct token_s;

//converts type_name enum to string
const char* type_name(token_type_t type);

//converts type_name enum to string
const char* group_name(token_group_t group);

//allocates token and inicializes it
token_s* init_t(char* val, uint line, uint col, token_type_t type, token_group_t group);

//prints token information
void print_t(token_s* t);
