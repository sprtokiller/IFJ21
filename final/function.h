#pragma once
#include "token.h"

typedef struct Span_token_type
{
	const token_type* begin;
	const token_type* end;
}Span_token_type;

inline size_t size_Span_token_type(Span_token_type* self)
{
	return (self->end - self->begin);
}
inline bool empty_Span_token_type(Span_token_type* self)
{
	return !(self->end - self->begin);
}

typedef const char* LPCSTR;

#define VECTOR_T LPCSTR
#include "Vector_T.h"

typedef struct FunctionDecl
{
	Span_token_type types;
	Span_token_type ret;
	Vector(LPCSTR) variables;
	bool proto;
	bool called;
}FunctionDecl;

inline void FunctionDecl_dtor(FunctionDecl* self)
{
	Vector_LPCSTR_dtor(&self->variables);
}
