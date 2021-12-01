#define CODE_IMPL
#include "codegen.h"

void Constructor(selfptr)
{
	Vector_String_ctor(&self->code);
	String_ctor(&self->global, NULL);
	CG_EndFunction(self);
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
