#pragma once
#include <stdio.h>

#include "common.h"
#include "token.h"
#include "Templates.h"
//char cluster, an item of a linked list
typedef struct coord_struct coord_s;
//
////gets word that can be used in token from stream
//token_s* get_t(FILE* f, LLC_s* llc, coord_s* coord);
//
//dont use this!
//just for editors autocomplete
struct coord_struct {
	uint32_t line;
	uint32_t column;
};

typedef struct Scanner Scanner;
struct Scanner
{
	FILE* source;
	uint32_t line;
	uint32_t column;
	int32_t sym;
	Vector(token) tk_stream;
};

void Scanner_ctor(Scanner* self, FILE* source);
void Scanner_dtor(Scanner* self);

Error _get_token(Scanner* self, token* tk);
