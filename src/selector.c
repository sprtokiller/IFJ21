#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "token.h"
#include "common.h"

const char* warning_num[] = { "Not number after comma!",  "Number is not ending with number!" };

//check if token is EOF
bool isEOF(token_s* t){
	if(t->val[1] == '\0')
		if (t->val[0] == EOF || t->val[0] == '\x1a') {
			t->type = eof;
			return true;
		}
	return false;
}

//skips all numbers in stream
void skipNum(char** val) {
	while (isdigit(**val))
		(*val)++;
}

//checks if string is number
bool isNum(token_s* t) {
	char* val = t->val;
	if (!isdigit(*val))
		return false;
	val++;
	skipNum(&val);
	//check for dot in decima number and skips it
	//floating point number
	if (*val == '.') {
		val++;
		//check for number after comma
		if (!isdigit(*val)) {
			t->warning = warning_num[0];
			t->type = err;
			return true;
		}
		val++;
		//skips to end number
		skipNum(&val);
		//check if number is ended right way
		if (*val == '\0') {
			t->type = lit;
			return true;
		}

		t->warning = warning_num[1];
		t->type = lit;
		return true;
	}
	//dicimal number
	else if (*val == '\0') {
		t->type = lit;
		return true;
	}
	t->warning = warning_num[1];
	t->type = err;
	return true;
}

//return length of array
#define lenght(x)  (sizeof(x) / sizeof((x)[0]))

bool isDataType(token_s* t) {
	const char* types[] = { "integer", "number", "string"};
	for (uint i = 0; i < lenght(types); i++)
	{
		if (strcmp(types[i], t->val) == 0) {
			t->type = dt;
			return true;
		}
	}
	return false;
}

bool isKeyWord(token_s* t) {
	const char* types[] = { "do", "else", "end", "function", "global",
		"if", "local", "nil", "require", "return", "then", "while"};
	for (uint i = 0; i < lenght(types); i++)
	{
		if (strcmp(types[i], t->val) == 0) {
			t->type = kw;
			return true;
		}
	}
	return false;
}

bool isBracket(token_s* t) {
	const char* types[] = { "(", ")", "[", "]", "{", "}"};
	for (uint i = 0; i < lenght(types); i++)
	{
		if (strcmp(types[i], t->val) == 0) {
			t->type = br;
			return true;
		}
	}
	return false;
}

// TODO: add rest of operators
bool isOperator(token_s* t) {
	const char* types[] = { "=", "+=", "-=", "*=", "/=", "%="};
	for (uint i = 0; i < lenght(types); i++)
	{
		if (strcmp(types[i], t->val) == 0) {
			t->type = op;
			return true;
		}
	}
	return false;
}

// TODO: implement
bool isLable(token_s* t) {
	return false;
}

// TODO: implement
bool isSemicolum(token_s* t) {
	return false;
}

// TODO: implement
bool isComma(token_s* t) {
	return false;
}

// TODO: implement
bool isAssigment(token_s* t) {
	return false;
}

//Returns type of token based on recived value
// TODO: do check for expession, if it follows rules in specification 
// TODO: check if switch covers all options
void selector(token_s* t) {
	if (isEOF(t))
		return;// eof
	else if (isNum(t))
		return;// lit
	else if (isDataType(t))
		return;// dt
	else if (isKeyWord(t))
		return;// kw
	else if (isBracket(t))
		return;// br
	else if (isOperator(t))
		return;// op
	else if (isLable(t))
		return;// lab
	else if (isSemicolum(t))
		return;// sem
	else if (isComma(t))
		return;// com
	else if (isAssigment(t))
		return;// as

	t->type = id;
	return;// id
}
