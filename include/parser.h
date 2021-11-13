#pragma once
#include "scanner.h"

#pragma push_macro("c_class")
#undef c_class
#define c_class Parser
typedef struct c_class c_class;

struct Parser
{
	
	Scanner scan;
};

void Constructor(selfptr);
void Destructor(selfptr);

void Start();

#ifndef PARSER_IMPL
#pragma pop_macro("c_class")
#endif // !PARSER_IMPL
