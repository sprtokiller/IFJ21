#pragma once
#include "Templates.h"
#include "error.h"

struct Scanner;
struct SemanticAnalyzer;


#pragma push_macro("c_class")
#undef c_class
#define c_class ExpressionAnalyzer
typedef struct ExpressionAnalyzer ExpressionAnalyzer;

struct ExpressionAnalyzer
{
	Vector(Node) ast;
};

void Constructor(ExpressionAnalyzer* self);
void Destructor(ExpressionAnalyzer* self);

Error Execute(ExpressionAnalyzer* self, struct Scanner* scanner);
token_type GetExpType(Vector(Node)* ast, struct SemanticAnalyzer* analyzer, Error* err);
void GenerateExpression(Vector(Node)* self, String* to);

#ifndef EXPRA_IMPL
#pragma pop_macro("c_class")
#endif
