#pragma pop_macro("HASH_T")

#ifdef HASH_T 
#include "common.h"
#include <string.h>
#include <malloc.h>
#include <memory.h>

#undef c_class
#define c_class HashMap(HASH_T)

#ifndef __HASH_SINGLE
#define __HASH_SINGLE
//fake hash function
static uint32_t hash(const char* id)
{
	uint32_t xhash = 0;
	while (*id)
		xhash += *id++ * 7919 ^ 44;
	return xhash % TABLE_SIZE;
}

#endif // !__HASH_SINGLE

extern inline void Template(_Destroy_single)(HASH_T* element);

void Constructor(selfptr)
{
	memset(self, 0, sizeof(*self));
}
static void Template(list_collapse)(Template(item)* list)
{
	if (!list)return;
	Template(list_collapse)(list->next_synonym);
	String_dtor(&list->identifier);
	Template(_Destroy_single)(&list->value);
	free(list);
}
void Destructor(selfptr)
{
	for (size_t i = 0; i < TABLE_SIZE; i++)
		Template(list_collapse)((*self)[i]);
}
HASH_T* Template(emplace)(selfptr, const char* id)
{
	uint32_t index = hash(id);
	Template(item)** x = (*self) + index;
	if (*x) {
		do {
			if (!strcmp(c_str(&(*x)->identifier), id))
				return NULL;
		} while (*(x = &(*x)->next_synonym));
	}

	//root is empty or not in list
	Template(item)* item = calloc(sizeof(Template(item)), 1);
	ALLOC_CHECK(item);
	String_ctor(&item->identifier, id);
	*x = item;

	return &item->value;
}
HASH_T* Template(find)(selfptr, const char* id)
{
	uint32_t index = hash(id);
	Template(item)* x = (*self)[index];

	if (x) {
		do {
			if (!strcmp(c_str(&x->identifier), id))return &x->value;
		} while ((x = x->next_synonym));
	}
	return NULL;
}

//void Sym_table_print(Sym_table* self){
//	for (size_t i = 0; i < TABLE_SIZE; i++) {
//		hashtable_item* item = (*self)[i];
//		while (item)
//		{
//			d_msg("Item id %s", item->identifier);
//			d_msg("     tt %s", token_type_name(item->token_type));
//			d_msg("     fc %i", item->frame_count);
//			item = item->next_synonym;
//		}
//	}
//}

#undef HASH_T
#include "symtable.c"
#endif