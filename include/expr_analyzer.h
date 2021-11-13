#pragma once
#include "Templates.h"

struct Scanner;

typedef enum {
	S,  // shift           (<)
	R,  // reduce          (>)
	E,  // equal           (=)
	X   // nothing - error ( )
}ExprAction;

#pragma push_macro("c_class")
#undef c_class
#define c_class ExpressionAnalyzer
typedef struct ExpressionAnalyzer ExpressionAnalyzer;

struct ExpressionAnalyzer
{
	Vector(Node) ast;
};

void Constructor(ExpressionAnalyzer* self, struct Scanner* scanner);
void Destructor(ExpressionAnalyzer* self);

#ifndef EXPRA_IMPL
#pragma pop_macro("c_class")
#endif
