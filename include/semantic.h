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
	Vector(Sym_table) symtab; //variables and functions
	size_t level; //scope level
};

inline bool SA_IsGlobal(selfptr)
{
	return self->level == 0;
}


#ifndef SEMANTIC_IMPL
#pragma pop_macro("c_class")
#endif