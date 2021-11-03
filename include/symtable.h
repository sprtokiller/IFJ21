#pragma once
#include "XString.h"
#include "token.h"

#define TABLE_SIZE 100 //Change this later

//A symbol, represented as a hash table item
typedef struct hashtable_v_item hashtable_v_item;

//dont use this!
//just for editors autocomplete
struct hashtable_v_item
{
	String identifier;
	struct hashtable_v_item_struct* next_synonym;
	token_type token_type;
	uint32_t frame_count;
};

typedef hashtable_v_item* Sym_v_table[TABLE_SIZE];

void Sym_v_table_dtor(Sym_v_table* self);

void Sym_v_table_insert(Sym_v_table* self, char* id, token_type token_type, uint32_t frame_count);

hashtable_v_item* Sym_v_table_find(Sym_v_table* self, char* id);

uint32_t hash(char* id);