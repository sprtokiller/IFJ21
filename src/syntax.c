#include "..\include\syntax.h"
#include "Templates.h"
#include <stdio.h>
#include <malloc.h>


typedef struct reqStmt
{
	Implements(IASTElement);
	bool valid;
	token arg;
}reqStmt;

void reqStmt_ctor(reqStmt* self);
void reqStmt_dtor(reqStmt* self);

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
///////////////////////////////////////////////////////

typedef struct blockPart
{
	Implements(IASTElement);
	bool valid : 1;
	IASTElement** active;
	Vector(pIASTElement) block;
}blockPart;

void blockPart_ctor(blockPart* self);
void blockPart_dtor(blockPart* self);

bool blk_append(blockPart* self, token* tk)
{
	if (!self->active)
	{
		if (tk->type == tt_end)return self->valid = true;
		*push_back_Vector_pIASTElement(&self->block) =
			MakeStatement(tk->type);
		self->active = back_Vector_pIASTElement(&self->block);
	}
	if ((*self->active) && (*self->active)->append(self->active, tk)) {
		self->active = NULL;
	}
	return false;
}
void blk_print(blockPart* self)
{
	for (size_t i = 0; i < size_Vector_pIASTElement(&self->block); i++) {
		IASTElement** el = at_Vector_pIASTElement(&self->block, i);
		(*el)->print(el);
	}
}

static const struct IASTElement vfptr_blk = (IASTElement)
{
	blk_append,
	blk_print,
	blockPart_dtor
};
void blockPart_ctor(blockPart* self)
{
	self->method = &vfptr_blk;
	self->valid = false;
	self->active = NULL;
	Vector_pIASTElement_ctor(&self->block);
}
void blockPart_dtor(blockPart* self)
{
	Vector_pIASTElement_dtor(&self->block);
}
///////////////////////////////////////////////////////

typedef struct funcDecl
{
	Implements(IASTElement);
	bool valid : 1;
	bool finished_args : 1;
	bool has_ret : 1;
	bool fills_block : 1;
	token name;
	Vector(token) args;
	Vector(token_type) types;
	Vector(token_type) ret;
	blockPart block;
}funcDecl;

void funcDecl_ctor(funcDecl* self);
void funcDecl_dtor(funcDecl* self);

bool fd_append(funcDecl* self, token* tk)
{
	if (self->fills_block) 
		return self->valid = blk_append(&self->block, tk);

	switch (tk->type)
	{
	case tt_identifier:
		if (self->finished_args)
		{
			self->fills_block = true;
			blk_append(&self->block, tk);
			break;
		}
		if (self->name.type == tt_err) {
			token_move_ctor(&self->name, tk);
			break;
		}
		token_move_ctor(push_back_Vector_token(&self->args), tk);
		break;
	case tt_left_parenthese:
		break;
	case tt_right_parenthese:
		self->finished_args = true;
		break;
	case tt_colon:
		if (self->finished_args)
			self->has_ret = true;
		break;
	default:
		if (is_type(tk->type)) {
			if (!self->finished_args) {
				*push_back_Vector_token_type(&self->types) = tk->type;
				break;
			}
			if (self->has_ret) {
				*push_back_Vector_token_type(&self->ret) = tk->type;
				break;
			}
		}
		self->fills_block = true;
		blk_append(&self->block, tk);
		break;
	}
	return false;
}
void fd_print(funcDecl* self)
{
	printf("function %s(", c_str(&self->name.sval));
	for (size_t i = 0; i < size_Vector_token_type(&self->types); i++)
	{
		printf("%s:", c_str(&at_Vector_token(&self->args, i)->sval));
		printf("%s ", token_type_name(*at_Vector_token_type(&self->types, i)));
	}
	putchar(')');
	if (!self->has_ret)return;
	putchar(':');
	for (size_t i = 0; i < size_Vector_token_type(&self->ret); i++) {
		printf("%s ", token_type_name(*at_Vector_token_type(&self->ret, i)));
	}
}

static const struct IASTElement vfptr_fd = (IASTElement)
{
	fd_append,
	fd_print,
	funcDecl_dtor
};
void funcDecl_ctor(funcDecl* self)
{
	self->method = &vfptr_fd;
	self->valid = false;
	self->finished_args = false;
	self->fills_block = self->has_ret = false;

	Vector_token_ctor(&self->args);
	Vector_token_type_ctor(&self->types);
	Vector_token_type_ctor(&self->ret);
	blockPart_ctor(&self->block);
}
void funcDecl_dtor(funcDecl* self)
{
	blockPart_dtor(&self->block);
	Vector_token_type_dtor(&self->ret);
	Vector_token_type_dtor(&self->types);
	Vector_token_dtor(&self->args);
}

IASTElement* MakeStatement(token_type type)
{
	IASTElement* out = NULL;
	switch (type)
	{
	case tt_require:
		out = calloc(sizeof(reqStmt), 1);
		reqStmt_ctor(out);
		break;
	case tt_function:
		out = calloc(sizeof(funcDecl), 1);
		funcDecl_ctor(out);
		break;
	default:
		break;
	}
	return out;
}
