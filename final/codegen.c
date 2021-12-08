#define CODE_IMPL
#include "codegen.h"
#include "builtin.h"

void Constructor(selfptr, HashMap(FunctionDecl)* funcs, bool has_pow)
{
	Vector_String_ctor(&self->code);
	String_ctor(&self->global, NULL);
	CG_EndFunction(self);
	self->funcs = funcs;
	self->has_pow = has_pow;
}
void Destructor(selfptr)
{
	Vector_String_dtor(&self->code);
	String_dtor(&self->global);
}

String* CG_AddFunction(selfptr)
{
	self->current = push_back_Vector_String(&self->code);
	String_ctor(self->current, NULL);
	return self->current;
}

String* CG_EndFunction(selfptr)
{
	return self->current = &self->global;
}

void CG_AddBuiltins(selfptr)
{
	FunctionDecl* fd = NULL;
	
	#define X(a)\
	if ((fd = find_htab_FunctionDecl(self->funcs, #a)) && (fd->called))\
		append_str(&self->global, x##a);
	LIST_BUILTINS()
	#undef X	
	if (self->has_pow)
		append_str(&self->global, xpow);
}