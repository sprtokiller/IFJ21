#include "symtable.h"
#include "common.h"
#include "stdlib.h"
#include <string.h>

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

	//root is empty
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
		} while (x = x->next_synonym);
	}
	return NULL;
}


