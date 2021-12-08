#pragma once
#include <stdio.h>
#include "common.h"
#include "Templates.h"

typedef struct Scanner Scanner;
struct Scanner
{
	FILE* source;
	uint32_t line;
	uint32_t column;
	uint32_t prev_state;
	int32_t sym;

	Vector(token) tk_stream;
};

void Scanner_ctor(Scanner* self, FILE* source);
void Scanner_dtor(Scanner* self);

Error get_token(Scanner* self, token* tk);

void unget_token(Scanner* self, token* tk);
