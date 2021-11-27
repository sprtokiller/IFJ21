#define SEMANTIC_IMPL
#include "semantic.h"

extern inline bool SA_IsGlobal(selfptr);

void Constructor(selfptr)
{
	self->level = 0;
	htab_FunctionDecl_ctor(&self->funcs);
}
void Destructor(selfptr)
{
	htab_FunctionDecl_dtor(&self->funcs);
}