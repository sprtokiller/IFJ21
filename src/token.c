#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "token.h"
#include "common.h"

//skips all numbers
void skipNum(char** val) {
	while (isdigit(**val))
		(*val)++;
}

const char* warning_name[] = { "Not number after comma!" };

//checks if string is number
// TODO: float is not fully implemented
bool isNum(token_s* t, char* val) {
	if (!isdigit(*val))
		return false;
	val++;
	skipNum(&val);
	//check for dot in decima number and skips it
	//floating point number
	if (*val == '.') {
		val++;
		if (!isdigit(*val)) {
			t->warning = warning_name[0];
			return false;
		}
		val++;
		skipNum(&val);
		if (*val == '\0')
			return true;
	}
	//dicimal number
	else if (*val == '\0')
		return true;
	return false;
}

//Returns type of token based on recived value
// TODO: implement "tiny" switch
token_t selector(token_s* t) {
	char* val = t->val;
	if (val[0] == EOF || val[0] == '\x1a')
		return eof;
	else if (isNum(t, val))
		return lit;
	else
		return id;
}

//allocates token and inicializes it
token_s* init_t(char* val, uint line, uint col) {
	token_s* t = malloc(sizeof(token_s));
	if (!t) {
		e_msg("Alocation failed.\n");
		return NULL;
	}
	// TODO: add define to disable debug info for faster scenner
	t->warning = NULL;
	t->line = line;
	t->colum = col;

	t->val = val;
	t->type = selector(t);
	return t;
}

//converts enum to string
const char* type_name(token_t type) {
	switch (type) {
	case id:
		return "idtifier";
	case dt:
		return "data type";
	case kw:
		return "key word";
	case lit:
		return "literal";
	case br:
		return "bracket";
	case bl:
		return "block";
	case op:
		return "operand";
	case lab:
		return "lable";
	case sem:
		return "semicolum";
	case com:
		return "comma";
	case as:
		return "asigment";
	case eof:
		return "end of file";
	default:
		return "error";
	}
}

//prints token information
// TODO implement and uncoment
void print_t(token_s* t) {
	d_msg("type:   %s", type_name(t->type));
	d_msg("val:    %s", t->val);
	//d_msg("line:   %i", t->line);
	//d_msg("colum:  %i", t->colum);
	if (t->warning != NULL)
		w_msg("%s", t->warning);
}
