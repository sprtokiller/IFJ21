#include "..\include\syntax.h"
#include <stdio.h>

bool rq_append(reqStmt* self, token* tk)
{
	token_move_ctor(&self->arg, tk);
	return self->valid = true;
}
void rq_print(reqStmt* self)
{
	printf("require %s", c_str(&self->arg.sval));
}

static const struct IASTElement vfptr_rq = (IASTElement)
{
	rq_append,
	rq_print,
	reqStmt_dtor
};
void reqStmt_ctor(reqStmt* self)
{
	self->method = &vfptr_rq;
	self->valid = false;
}
void reqStmt_dtor(reqStmt* self)
{
	if (self->valid)
		token_dtor(&self->arg);
}
