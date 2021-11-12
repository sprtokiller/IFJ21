#pragma once
#include "scanner.h"

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
#undef c_class
#endif // !PARSER_IMPL
