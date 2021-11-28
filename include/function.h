#pragma once
#include "token.h"

typedef struct Span_token_type
{
	token_type* begin;
	token_type* end;
}Span_token_type;

typedef struct FunctionDecl
{
	Span_token_type types;
	Span_token_type ret;
	bool proto;
	bool called;
}FunctionDecl;
