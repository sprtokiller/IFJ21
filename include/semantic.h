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
	Vector(HashMap(token_type)) scopes;
	HashMap(token_type)* current;
	size_t level; //scope level
};

inline bool SA_IsGlobal(selfptr)
{
	return self->level == 0 || self->level == (size_t)-1;
}

void SA_AddScope(selfptr);
void SA_ResignScope(selfptr);

bool SA_AddFunction(selfptr, Vector(token_type)* args, Vector(token_type)* rets, const char* id, bool prototype);
bool SA_AddVariable(selfptr, const char* id, token_type type);
token_type SA_FindVariable(selfptr, const char* id);
FunctionDecl* SA_FindFunction(selfptr, const char* id);
bool SA_Final(const selfptr);

token_type GetExpType(const Vector(Node)* ast, struct SemanticAnalyzer* analyzer);//convenience


#ifndef SEMANTIC_IMPL
#pragma pop_macro("c_class")
#endif