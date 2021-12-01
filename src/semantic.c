#define SEMANTIC_IMPL
#include "semantic.h"

extern inline bool SA_IsGlobal(selfptr);

void Constructor(selfptr)
{
	self->level = (size_t)-1;
	htab_FunctionDecl_ctor(&self->funcs);
	self->scopes = (Vector(HashMap(Variable))){ 0 };
	FunctionDecl* fdec = NULL;
#define X(a) fdec = emplace_htab_FunctionDecl(&self->funcs, #a); *fdec = fd##a;
	LIST_BUILTINS()
#undef X
}
void Destructor(selfptr)
{
	htab_FunctionDecl_dtor(&self->funcs);
	Vector_htab_Variable_dtor(&self->scopes);
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
	self->current = push_back_Vector_htab_Variable(&self->scopes);
	htab_Variable_ctor(self->current);
}

void SA_ResignScope(selfptr)
{
	self->level--;
	pop_back_Vector_htab_Variable(&self->scopes);
	if (empty_Vector_htab_Variable(&self->scopes)) { self->current = NULL; return; }
	self->current = back_Vector_htab_Variable(&self->scopes);
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
	self->curr_func = fdec;
	return true;
}

void SA_LeaveFunction(selfptr)
{
	self->curr_func = NULL;
}

bool SA_AddVariable(selfptr, String* id, token_type type, bool has_value, bool global)
{
	Variable* xtok = find_htab_Variable(self->current, c_str(id));
	Variable* tok = emplace_htab_Variable(self->current, id);
	if (!tok)return false; //var already exists, push failed

	if (global)
		prepend_str(id, "GF@");
	else
		prepend_str(id, "LF@");

	if (xtok)
	{
		char x[sizeof(void*) * 2 + 2] = {0};
		sprintf(x, "_%p", id);
		append_str(id, x);
	}

	*tok = (Variable){ c_str(id), type, has_value};
	return true;
}

Variable* SA_FindVariable(selfptr, const char* id)
{
	Variable* tok = NULL;
	for (htab_Variable* i = self->current; i > self->scopes.data_; i--)
		if ((tok = find_htab_Variable(i, id)))return tok;
	return NULL;
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

extern inline bool FitsType(token_type t1, token_type t2);
