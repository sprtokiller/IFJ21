#include <string.h>

#include "symtable.h"
#include "common.h"
#include "stdlib.h"
#include "scanner.h"

//fake hash function
static uint32_t hash(const char* id)
{
	uint32_t xhash = 0;
	while (*id)
		xhash+=*id++ * 7919 ^ 44;
	return xhash % TABLE_SIZE;
}

static void list_collapse(hashtable_item* list)
{
	if (!list)return;
	list_collapse(list->next_synonym);
	String_dtor(&list->identifier);
	free(list);
}

void Sym_table_dtor(Sym_table* self)
{
	for (size_t i = 0; i < TABLE_SIZE; i++)
		list_collapse((*self)[i]);
}

void Sym_table_insert(Sym_table* self, const char* id, token_type token_type, uint32_t frame_count)
{
	uint32_t index = hash(id);
	hashtable_item** x = (*self) + index;
	if (*x) {
		do {
			if (!strcmp(c_str(&(*x)->identifier), id))return;
		} while (*(x = &(*x)->next_synonym));
	}

	//root is empty or not in list
	hashtable_item* item = (hashtable_item*)malloc(sizeof(hashtable_item));
	if (!item)
		e_exit("Hashable_item allocation failed!");
	String_ctor(&item->identifier, id);
	item->token_type = token_type;
	item->frame_count = frame_count;
	item->next_synonym = NULL;

	*x = item;
}

hashtable_item* Sym_table_find(Sym_table* self, const char* id)
{
	uint32_t index = hash(id);
	hashtable_item* x = (*self)[index];

	if (x) {
		do {
			if (!strcmp(c_str(&x->identifier), id))return x;
		} while ((x = x->next_synonym));
	}
	return NULL;
}


void Sym_table_print(Sym_table* self){
	for (size_t i = 0; i < TABLE_SIZE; i++) {
		hashtable_item* item = (*self)[i];
		while (item)
		{
			d_msg("Item id %s", item->identifier);
			d_msg("     tt %s", token_type_name(item->token_type));
			d_msg("     fc %i", item->frame_count);
			item = item->next_synonym;
		}
	}
}

void Sym_table_fill(Sym_table* self, FILE* stream) {
	//Scanner sc;
	//Error e;
	//Scanner_ctor(&sc, stream);
	//Scanner_run(&sc, &e);
	//for (size_t i = 0; i < size_Vector_token(&(sc.tk_stream)); i++) {
	//	///@TODO: Fix this sprintf and implement proper frameCounter pls
	//	static char s[20];
	//	sprintf(s, "ReplaceMe %llu", i);
	//	token* t = at_Vector_token(&(sc.tk_stream), i);
	//	Sym_table_insert(self, s, t->type, t->column);
	//}
	////Scanner_print(&sc);
	//Scanner_dtor(&sc);
}
