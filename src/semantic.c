#define SEMANTIC_IMPL
#include "semantic.h"

extern inline bool SA_IsGlobal(selfptr);

void Constructor(selfptr)
{
	self->level = (size_t)-1;
	htab_FunctionDecl_ctor(&self->funcs);
	self->scopes = (Vector(HashMap(token_type))){ 0 };
}
void Destructor(selfptr)
{
	htab_FunctionDecl_dtor(&self->funcs);
	Vector_htab_token_type_dtor(&self->scopes);
}

bool Span_EQ(Span_token_type a, Span_token_type b)
{
	if ((a.end - a.begin) != (b.end - b.begin))return false;
	while (a.begin != a.end) {
		if (*a.begin++ != *b.begin++)return false;
	}
	return true;
}

void SA_AddScope(selfptr)
{
	self->level++;
	self->current = push_back_Vector_htab_token_type(&self->scopes);
	htab_token_type_ctor(self->current);
}

void SA_ResignScope(selfptr)
{
	self->level--;
	pop_back_Vector_htab_token_type(&self->scopes);
	if (empty_Vector_htab_token_type(&self->scopes)) { self->current = NULL; return; }
	self->current = back_Vector_htab_token_type(&self->scopes);
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
	fdec->called = false;
	fdec->proto = prototype;
	fdec->types = (Span_token_type){ args->data_, args->end_ };
	fdec->ret = (Span_token_type){ rets->data_, rets->end_ };
	return true;
}

bool SA_AddVariable(selfptr, const char* id, token_type type)
{
	token_type* tok = emplace_htab_token_type(self->current, id);
	if (!tok)return false;
	*tok = type;
	return true;
}

token_type SA_FindVariable(selfptr, const char* id)
{
	token_type* tok = NULL;
	for (htab_token_type* i = self->current; i < self->scopes.data_; i--)
		if (tok = find_htab_token_type(i, id))return *tok;
	return tt_err;
}

FunctionDecl* SA_FindFunction(selfptr, const char* id)
{
	return find_htab_FunctionDecl(&self->funcs, id);
}


bool list_func_overview(item_htab_FunctionDecl* item)
{
	if (!item)return true;
	return list_func_overview(item->next_synonym) && !(item->value.called && item->value.proto);
}
bool SA_Final(const selfptr)
{
	bool ret = true;
	for (size_t i = 0; i < TABLE_SIZE; i++)
		if (self->funcs[i])
			ret &= list_func_overview(self->funcs[i]);
	if (!ret) printf("Some function implementation is missing\n");
	return ret;
}
