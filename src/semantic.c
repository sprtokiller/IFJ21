#define SEMANTIC_IMPL
#include "semantic.h"

extern inline bool SA_IsGlobal(selfptr);

void Constructor(selfptr)
{
	self->level = 0;
}
void Destructor(selfptr)
{
	Vector_Sym_table_dtor(&self->symtab);
}