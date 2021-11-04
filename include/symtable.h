#pragma once
#include "XString.h"
#include "token.h"

#define TABLE_SIZE 100 //Change this later

//A symbol, represented as a hash table item
typedef struct hashtable_item hashtable_item;

//dont use this!
//just for editors autocomplete
struct hashtable_item
{
	String identifier;
	token_type token_type;
	uint32_t frame_count;

	hashtable_item* next_synonym;
};

typedef hashtable_item* Sym_table[TABLE_SIZE];

void Sym_table_dtor(Sym_table* self);

void Sym_table_insert(Sym_table* self, const char* id, token_type token_type, uint32_t frame_count);

hashtable_item* Sym_table_find(Sym_table* self, const char* id);
