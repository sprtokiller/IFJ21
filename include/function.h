#pragma once
#include "token.h"

typedef struct Span_token_type
{
	const token_type* begin;
	const token_type* end;
}Span_token_type;

inline bool size_Span_token_type(Span_token_type* self)
{
	return (self->end - self->begin);
}
inline bool empty_Span_token_type(Span_token_type* self)
{
	return !(self->end - self->begin);
}


typedef struct FunctionDecl
{
	Span_token_type types;
	Span_token_type ret;
	bool proto;
	bool called;
}FunctionDecl;
