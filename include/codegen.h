#pragma once
#include "Templates.h"

#pragma push_macro("c_class")
#undef c_class
#define c_class CodeGen
typedef struct c_class c_class;

struct c_class
{
	HashMap(FunctionDecl)* funcs;
	Vector(String) code;
	String* current;
	String global;
};


void Constructor(selfptr, HashMap(FunctionDecl)* funcs);
void Destructor(selfptr);

String* CG_AddFunction(selfptr);
String* CG_EndFunction(selfptr);

#ifndef CODE_IMPL
#pragma pop_macro("c_class")
#endif