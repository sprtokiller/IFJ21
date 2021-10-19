#pragma once
#include "token.h"
#include "llc.h"
#include "scanner.h"

//gets word that can be used in token from stream
token_s* get_t(FILE* f, LLC_s* llc, coord_s* coord);

//dont use this!
//just for editors autocomplete
struct coord_struct {
	uint line;
	uint column;
};

//char cluster, an item of a linked list
typedef struct coord_struct coord_s;
