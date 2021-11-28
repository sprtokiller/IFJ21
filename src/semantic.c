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

bool Span_EQ(Span_token_type a, Span_token_type b)
{
	if ((a.end - a.begin) != (b.end - b.begin))return false;
	while (a.begin != a.end){
		if (*a.begin++ != *b.begin++)return false;
	}
	return true;
}

bool SA_AddFunction(selfptr, Vector(token_type)* args, Vector(token_type)* rets, const char* id, bool prototype)
{
	FunctionDecl* fdec = emplace_htab_FunctionDecl(&self->funcs, id);
	if (!fdec)
	{
		fdec = find_htab_FunctionDecl(&self->funcs, id);
		if (fdec->proto == prototype && !prototype)return false;
		
		return !(!Span_EQ(fdec->types, (Span_token_type) { args->data_, args->end_ }) ||
			!Span_EQ(fdec->ret, (Span_token_type) { rets->data_, rets->end_ }));
	}
	fdec->proto = prototype;
	fdec->types = (Span_token_type){args->data_, args->end_};
	fdec->ret = (Span_token_type){rets->data_, rets->end_};
	return true;
}
