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

bool SA_AddFunction(selfptr, Vector(token_type)* args, Vector(token_type)* rets, const char* id, bool prototype)
{
	FunctionDecl* fdec = emplace_htab_FunctionDecl(&self->funcs, id);
	if (!fdec)
	{
		fdec = find_htab_FunctionDecl(&self->funcs, id);
		if (prototype)return true;
		if (fdec->proto == prototype && !prototype)return false;
	}
	fdec->proto = prototype;
	fdec->types = (Span_token_type){args->data_, args->end_};
	fdec->ret = (Span_token_type){rets->data_, rets->end_};
	return true;
}
