#define NODE_IMPL
#include "ast.h"


void Constructor(selfptr)
{
	self->valid = 0;
	self->left = self->right = NULL;
}
void Destructor(selfptr)
{
	if(self->valid)
		token_dtor(&self->core);
}

token* Node_emplace(selfptr)
{
	Destructor(self);
	self->valid = true;
	return &self->core;
}