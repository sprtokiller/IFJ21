#pragma once

#define VECTOR_T token_type
#include "VLib/Vector_T.h"

typedef struct FunctionDecl
{
	Vector(token_type) types;
	Vector(token_type) ret;
}FunctionDecl;

inline void FunctionDecl_dtor(FunctionDecl* self)
{
	Vector_token_type_dtor(&self->types);
	Vector_token_type_dtor(&self->ret);
}