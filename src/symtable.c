#include "symtable.h"

void Sym_v_table_dtor(Sym_v_table* self)
{
	return;
}

void Sym_v_table_insert(Sym_v_table* self, char* id, token_type token_type, uint32_t frame_count)
{
	return;
}

hashtable_v_item* Sym_v_table_find(Sym_v_table* self, char* id)
{
	return NULL;
}

//fake hash function
uint32_t hash(char* id)
{
	if (id)
	{
		return (uint32_t)id[0] % TABLE_SIZE;
	}
	return 0;
}
