#pragma once
#include "token.h"

typedef struct IASTElement
{
	bool(*append)(struct IASTElement** self, token* tk);
	void(*print)(struct IASTElement** self);
	void(*dtor)(struct IASTElement** self);
}IASTElement,*pIASTElement;

inline void pIASTElement_dtor(pIASTElement*self)
{
	(*self)->dtor(self);
}

IASTElement* MakeStatement(token_type type);

