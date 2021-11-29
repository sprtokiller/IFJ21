#pragma once
#include "Templates.h"

#pragma push_macro("c_class")
#undef c_class
#define c_class SemanticAnalyzer
typedef struct c_class c_class;

void Constructor(selfptr);
void Destructor(selfptr);


struct SemanticAnalyzer
{
	HashMap(FunctionDecl) funcs; //functions
	Vector(HashMap(Variable)) scopes;
	HashMap(Variable)* current;
	FunctionDecl* curr_func;
	size_t level; //scope level
};

inline bool SA_IsGlobal(selfptr)
{
	return self->level == 0 || self->level == (size_t)-1;
}

void SA_AddScope(selfptr);
void SA_ResignScope(selfptr);

bool SA_AddFunction(selfptr, Vector(token_type)* args, Vector(token_type)* rets, const char* id, bool prototype);
void SA_LeaveFunction(selfptr);
bool SA_AddVariable(selfptr, const char* id, token_type type, bool has_value);
Variable* SA_FindVariable(selfptr, const char* id);
FunctionDecl* SA_FindFunction(selfptr, const char* id);
bool SA_Final(const selfptr);
inline bool FitsType(token_type t1, token_type t2)
{
	return t1 == t2 || (t1 == tt_number && t2 == tt_integer);
}

token_type GetExpType(const Vector(Node)* ast, struct SemanticAnalyzer* analyzer);//convenience


#ifndef SEMANTIC_IMPL
#pragma pop_macro("c_class")
#endif