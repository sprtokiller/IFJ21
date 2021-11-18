#pragma once
#include "scanner.h"
#include "expr_analyzer.h"

#pragma push_macro("c_class")
#undef c_class
#define c_class Parser
typedef struct c_class c_class;

struct Parser
{
	Vector(token_type) stack;
	ExpressionAnalyzer exp;
	Scanner scan;
};

void Constructor(selfptr, FILE* file);
void Destructor(selfptr);

Error Start(selfptr);

#ifndef PARSER_IMPL
#pragma pop_macro("c_class")
#endif // !PARSER_IMPL
