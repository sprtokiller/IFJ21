#pragma once
#include "Templates.h"
#include <malloc.h>

typedef struct IASTElement
{
	bool(*append)(struct IASTElement* self, token* tk);
	void(*print)(struct IASTElement* self);
	void(*dtor)(struct IASTElement* self);
}IASTElement;

typedef struct reqStmt
{
	Implements(IASTElement);
	bool valid;
	token arg;
}reqStmt;

void reqStmt_ctor(reqStmt* self);
void reqStmt_dtor(reqStmt* self);

inline IASTElement* MakeStatement(token_type type)
{
	IASTElement* out = NULL;
	switch (type)
	{
	case tt_require:
		out = calloc(sizeof (reqStmt), 1);
		reqStmt_ctor(out);
	default:
		break;
	}
	return out;
}

