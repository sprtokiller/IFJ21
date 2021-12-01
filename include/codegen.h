#pragma once
#include "Templates.h"

#pragma push_macro("c_class")
#undef c_class
#define c_class CodeGen
typedef struct c_class c_class;

struct c_class
{
	String global;
	Vector(String) code;
	String* current;
};


void Constructor(selfptr);
void Destructor(selfptr);

String* CG_AddFunction(selfptr);
String* CG_EndFunction(selfptr);

#ifndef CODE_IMPL
#pragma pop_macro("c_class")
#endif