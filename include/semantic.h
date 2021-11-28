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
	size_t level; //scope level
};

inline bool SA_IsGlobal(selfptr)
{
	return self->level == 0;
}

bool SA_AddFunction(selfptr, Vector(token_type)* args, Vector(token_type)* rets, const char* id, bool prototype);


#ifndef SEMANTIC_IMPL
#pragma pop_macro("c_class")
#endif