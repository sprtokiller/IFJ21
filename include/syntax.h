#pragma once
#include "token.h"

typedef enum
{
	s_await,
	s_accept,
	s_refused
}RetState;

typedef struct IASTElement
{
	RetState(*append)(struct IASTElement** self, token* tk);
	void(*print)(struct IASTElement** self);
	void(*dtor)(struct IASTElement** self);
}IASTElement,**ppIASTElement;

inline void ppIASTElement_dtor(ppIASTElement*self)
{
	(**self)->dtor(*self);
}

IASTElement** MakeStatement(token_type type);

