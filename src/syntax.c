#include "..\include\syntax.h"
#include "Templates.h"
#include <stdio.h>
#include <malloc.h>

static void PrintExpression(const Vector(Node)* vec)
{
	for (size_t i = 1; i < size_Vector_Node(vec); i++)
	{
		PrintNodeVal(at_Vector_Node(vec, i));
		putchar(' ');
	}
}


typedef struct reqStmt
{
	Implements(IASTElement);
	bool valid;
	token arg;
}reqStmt;

void reqStmt_ctor(reqStmt* self);
void reqStmt_dtor(reqStmt* self);

RetState rq_append(reqStmt* self, token* tk)
{
	if (tk->type == tt_require)
		return s_await;
	token_move_ctor(&self->arg, tk);
	self->valid = true;
	return s_accept;
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
#pragma region Block
typedef struct blockPart
{
	Implements(IASTElement);
	bool valid : 1;
	IASTElement** active;
	Vector(ppIASTElement) block;
}blockPart;

void blockPart_ctor(blockPart* self);
void blockPart_dtor(blockPart* self);

RetState blk_append(blockPart* self, token* tk)
{
begin:
	if (!self->active)
	{
		if (tk->type == tt_end) { self->valid = true; return s_accept; }
		*push_back_Vector_ppIASTElement(&self->block) =
			MakeStatement(tk->type);
		self->active = *back_Vector_ppIASTElement(&self->block);
	}
	if (!*self->active) return s_await;

	switch ((*self->active)->append(self->active, tk))
	{
	case s_refused: self->active = NULL; goto begin;
	case s_accept:
		self->active = NULL;
		/* fallthrough */
	case s_await:
	default:
		break;
	}
	return s_await;
}
void blk_print(blockPart* self)
{
	for (size_t i = 0; i < size_Vector_ppIASTElement(&self->block); i++) {
		IASTElement** el = *at_Vector_ppIASTElement(&self->block, i);
		if (*el)(*el)->print(el);
		putchar('\n');
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
	Vector_ppIASTElement_ctor(&self->block);
}
void blockPart_dtor(blockPart* self)
{
	Vector_ppIASTElement_dtor(&self->block);
}
#pragma endregion
///////////////////////////////////////////////////////
#pragma region Function
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

RetState fd_append(funcDecl* self, token* tk)
{
	if (self->fills_block)
		return self->valid = blk_append(&self->block, tk);

	switch (tk->type)
	{
	case tt_function:
		return s_await;
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
	if (self->has_ret)
	{
		putchar(':');
		for (size_t i = 0; i < size_Vector_token_type(&self->ret); i++) {
			printf("%s ", token_type_name(*at_Vector_token_type(&self->ret, i)));
		}
	}
	putchar('\n');
	blk_print(&self->block);
	printf("end");
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
	token_dtor(&self->name);
	blockPart_dtor(&self->block);
	Vector_token_type_dtor(&self->ret);
	Vector_token_type_dtor(&self->types);
	Vector_token_dtor(&self->args);
}
#pragma endregion
///////////////////////////////////////////////////////
#pragma region Local
typedef struct localStmt
{
	Implements(IASTElement);
	bool valid : 1;
	bool has_value : 1;
	String id;
	token_type type;
	Vector(Node) value;
}localStmt;

void localStmt_ctor(localStmt* self);
void localStmt_dtor(localStmt* self);

RetState ls_append(localStmt* self, token* tk)
{
	if (is_type(tk->type))
	{
		self->type = tk->type;
		return s_await;
	}
	switch (tk->type)
	{
	case tt_local:
		if (!empty_str(&self->id))
			return s_refused;
		break;
	case tt_identifier:
		if (empty_str(&self->id))
		{
			String_move_ctor(&self->id, &tk->sval);
			break;
		}
	case tt_colon:return s_await;
	case tt_assign:
		self->has_value = true;
		break;
	case tt_expression:
		Vector_Node_move_ctor(&self->value, (Vector(Node)*)tk->expression);
		self->valid = true;
		return s_accept;
	default:
		return s_refused;
	}
	return s_await;
}
void ls_print(localStmt* self)
{
	printf("local %s:%s", c_str(&self->id), token_type_name(self->type));
	if (self->has_value)
	{
		putchar(' ');
		putchar('=');
		for (size_t i = 1; i < size_Vector_Node(&self->value); i++)
		{
			putchar(' ');
			PrintNodeVal(at_Vector_Node(&self->value, i));
			putchar(' ');
		}
	}
}

static const struct IASTElement vfptr_ls = (IASTElement)
{
	ls_append,
	ls_print,
	localStmt_dtor
};
void localStmt_ctor(localStmt* self)
{
	self->method = &vfptr_ls;
	self->valid = self->has_value = 0;
	String_ctor(&self->id, NULL);
}
void localStmt_dtor(localStmt* self)
{
	String_dtor(&self->id);
	Vector_Node_dtor(&self->value);
}
#pragma endregion
///////////////////////////////////////////////////////
#pragma region Ass
typedef struct List_Node
{
	Vector(Node) expression;
	struct List_Node* next;
}List_Node;

void List_Node_dtor(List_Node* self)
{
	if (!self)return;
	Vector_Node_dtor(&self->expression);
	List_Node_dtor(self->next);
}

typedef struct List_exp
{
	List_Node* first;
	List_Node* last;
	size_t n;
}List_exp;

void List_exp_ctor(List_exp* self)
{
	self->first = self->last = NULL;
	self->n = 0;
}
void List_exp_dtor(List_exp* self)
{
	List_Node_dtor(self->first);
}
List_exp* push(List_exp* self)
{
	if (!self->last)
	{
		self->n = 1;
		return self->first = self->last = calloc(sizeof(List_Node), 1);
	}
	self->n++;
	return self->last->next = self->last = calloc(sizeof(List_Node), 1);
}

typedef struct AssOrFCall
{
	Implements(IASTElement);
	bool valid : 1;
	enum {
		ass_none, ass_ass, ass_fcall
	} op : 3; // 0-fcall 1-ass
	bool eq : 1;

	Vector(token) idents;
	List_exp expressions;
}AssOrFCall;

void AssOrFCall_ctor(AssOrFCall* self);
void AssOrFCall_dtor(AssOrFCall* self);

RetState afc_append(AssOrFCall* self, token* tk)
{
	switch (self->op)
	{
	case ass_none:
		switch (tk->type)
		{
		case tt_identifier:
		{
			token* ptr = push_back_Vector_token(&self->idents);
			token_move_ctor(ptr, tk);
			break;
		}
		case tt_assign:
		case tt_comma:
			self->op = ass_ass;
			break;
		case tt_left_parenthese:
			self->op = ass_fcall;
			break;
		default:
			break;
		}
		return s_await;
	case ass_fcall:
		switch (tk->type)
		{
		case tt_comma:
			break;
		case tt_expression:
			Vector_Node_move_ctor(push(&self->expressions),
				(Vector(Node)*)tk->expression);
			break;
		case tt_right_parenthese:
			self->valid = true;
			return s_accept;
		default:
			return s_refused;
		}
		return s_await;
	case ass_ass:
		switch (tk->type)
		{
		case tt_identifier:
		{
			if (self->eq) {
				self->valid = true; return s_refused;
			}
			token* ptr = push_back_Vector_token(&self->idents);
			token_move_ctor(ptr, tk);
			break;
		}
		case tt_comma:
			break;
		case tt_assign:
			self->eq = true;
			break;
		case tt_expression:
			Vector_Node_move_ctor(push(&self->expressions),
				(Vector(Node)*)tk->expression);
			break;
		default:
			self->valid = true;
			return s_refused;
		}
		return s_await;
	default:
		break;
	}
}
void afc_print(AssOrFCall* self)
{
	switch (self->op)
	{
	case ass_ass:
		for (size_t i = 0; i < size_Vector_token(&self->idents); i++) {
			if (i)putchar(',');
			token* el = at_Vector_token(&self->idents, i);
			printf("%s", c_str(&el->sval));
		}
		printf(" = ");
		//make explist print
		break;
	case ass_fcall:
		token* el = at_Vector_token(&self->idents, 0);
		printf("%s(", c_str(&el->sval));
		putchar(')');
		break;
	default:
		break;
	}
}

static const struct IASTElement vfptr_afc = (IASTElement)
{
	afc_append,
	afc_print,
	AssOrFCall_dtor
};
void AssOrFCall_ctor(AssOrFCall* self)
{
	self->method = &vfptr_afc;
	self->valid = false;
	List_exp_ctor(&self->expressions);
}
void AssOrFCall_dtor(AssOrFCall* self)
{
	Vector_token_dtor(&self->idents);
	List_exp_dtor(&self->expressions);
}
#pragma endregion
///////////////////////////////////////////////////////
#pragma region Program
typedef struct Program
{
	Implements(IASTElement);
	reqStmt req;
	blockPart global_block;
}Program;

void Program_ctor(Program* self);
void Program_dtor(Program* self);

RetState prg_append(Program* self, token* tk)
{
	if (!self->req.valid)
	{
		rq_append(&self->req, tk);
		return s_await;
	}
	if (tk->type == tt_eof)
	{
		self->global_block.valid = true;
		return s_accept;
	}
	return blk_append(&self->global_block, tk);
}
void prg_print(Program* self)
{
	rq_print(&self->req);
	putchar('\n');
	blk_print(&self->global_block);
}

static const struct IASTElement vfptr_prg = (IASTElement)
{
	prg_append,
	prg_print,
	reqStmt_dtor
};
void Program_ctor(Program* self)
{
	self->method = &vfptr_prg;
	reqStmt_ctor(&self->req);
	blockPart_ctor(&self->global_block);
}
void Program_dtor(Program* self)
{
	blockPart_dtor(&self->global_block);
	reqStmt_dtor(&self->req);
}
#pragma endregion
///////////////////////////////////////////////////////
#pragma region While
typedef struct While
{
	Implements(IASTElement);
	bool valid : 1;
	bool fills_block : 1;
	Vector(Node) expr;
	blockPart block;
}While;

void While_ctor(While* self);
void While_dtor(While* self);

RetState wh_append(While* self, token* tk)
{
	if (self->fills_block)
		return blk_append(&self->block, tk);

	switch (tk->type)
	{
	case tt_do:
		self->fills_block = true;
		return s_await;
	case tt_expression:
		Vector_Node_move_ctor(&self->expr,
			(Vector(Node)*)tk->expression);
		return s_await;
	case tt_while:
	default:
		return s_await;
	}
	return s_await;
}
void wh_print(While* self)
{
	printf("while ");
	PrintExpression(&self->expr);
	blk_print(&self->block);
	printf("end");
}

static const struct IASTElement vfptr_wh = (IASTElement)
{
	wh_append,
	wh_print,
	While_dtor
};
void While_ctor(While* self)
{
	self->method = &vfptr_wh;
	self->valid = false;
	self->fills_block = false;
	blockPart_ctor(&self->block);
}
void While_dtor(While* self)
{
	blockPart_dtor(&self->block);
	Vector_Node_dtor(&self->expr);
}
#pragma endregion
///////////////////////////////////////////////////////
#pragma region For
typedef struct For
{
	Implements(IASTElement);
	bool valid : 1;
	bool has_increment : 1;
	bool fills_block : 1;
	bool has_terminus : 1;
	bool has_expr : 1;
	token id;
	Vector(Node) expr;
	Vector(Node) terminus;
	Vector(Node) increment;
	blockPart block;
}For;

void For_ctor(For* self);
void For_dtor(For* self);

RetState for_append(For* self, token* tk)
{
	if (self->fills_block)
		return blk_append(&self->block, tk);

	switch (tk->type)
	{
	case tt_identifier:
		token_move_ctor(&self->id, tk);
		return s_await;
	case tt_comma:
	case tt_assign:
	case tt_for:return s_await;
	case tt_expression:
		if (!self->has_expr)
		{
			Vector_Node_move_ctor(&self->expr,
				(Vector(Node)*)tk->expression);
			self->has_expr = true;
			return s_await;
		}
		if (!self->has_terminus)
		{
			Vector_Node_move_ctor(&self->terminus,
				(Vector(Node)*)tk->expression);
			self->has_terminus = true;
			return s_await;
		}
		if (!self->has_increment)
		{
			Vector_Node_move_ctor(&self->terminus,
				(Vector(Node)*)tk->expression);
			self->has_increment = true;
			return s_await;
		}
		return s_await;
	case tt_do:
		self->fills_block = true;
	default:break;
	}
}
void for_print(For* self)
{
	printf("for %s = ", c_str(&self->id.sval));
	PrintExpression(&self->expr);
	putchar(',');
	PrintExpression(&self->terminus);
	if (self->has_increment)
	{
		putchar(',');
		PrintExpression(&self->increment);
	}

	blk_print(&self->block);
}

static const struct IASTElement vfptr_for = (IASTElement)
{
	for_append,
	for_print,
	For_dtor
};
void For_ctor(For* self)
{
	self->method = &vfptr_for;
	self->valid = false;
}
void For_dtor(For* self)
{
	token_dtor(&self->id);
	Vector_Node_dtor(&self->expr);
	Vector_Node_dtor(&self->terminus);
	Vector_Node_dtor(&self->increment);
	blockPart_dtor(&self->block);
}
#pragma endregion
///////////////////////////////////////////////////////
#pragma region Repeat
typedef struct Repeat
{
	Implements(IASTElement);
	bool valid : 1;
	bool fills_block : 1;
	Vector(Node) expr;
	blockPart block;
}Repeat;

void Repeat_ctor(Repeat* self);
void Repeat_dtor(Repeat* self);

RetState req_append(Repeat* self, token* tk)
{
	if (self->fills_block)
	{
		switch (tk->type)
		{
		case tt_until:
			self->block.valid = true;
			self->fills_block = false;
			return s_await;
		case tt_repeat:
			return s_await;
		default:
			break;
		}
		return blk_append(&self->block, tk);
	}

	switch (tk->type)
	{
	case tt_expression:
		Vector_Node_move_ctor(&self->expr,
			(Vector(Node)*)tk->expression);
		return s_accept;
	default:
		return s_await;
	}
}
void req_print(Repeat* self)
{
	printf("repeat\n");
	blk_print(&self->block);
	printf("until");
	PrintExpression(&self->expr);
}

static const struct IASTElement vfptr_req = (IASTElement)
{
	req_append,
	req_print,
	Repeat_dtor
};
void Repeat_ctor(Repeat* self)
{
	self->method = &vfptr_req;
	self->valid = false;
	self->fills_block = true;
	blockPart_ctor(&self->block);
}
void Repeat_dtor(Repeat* self)
{
	blockPart_dtor(&self->block);
	Vector_Node_dtor(&self->expr);
}
#pragma endregion
///////////////////////////////////////////////////////

IASTElement** MakeStatement(token_type type)
{
	IASTElement** out = NULL;
	switch (type)
	{
	case tt_function:
		out = calloc(sizeof(funcDecl), 1);
		funcDecl_ctor(out);
		break;
	case tt_local:
		out = calloc(sizeof(localStmt), 1);
		localStmt_ctor(out);
		break;
	case tt_identifier:
		out = calloc(sizeof(AssOrFCall), 1);
		AssOrFCall_ctor(out);
		break;
	case tt_while:
		out = calloc(sizeof(While), 1);
		While_ctor(out);
		break;
	case tt_for:
		out = calloc(sizeof(For), 1);
		For_ctor(out);
		break;
	case tt_repeat:
		out = calloc(sizeof(Repeat), 1);
		Repeat_ctor(out);
		break;
	default:
		break;
	}
	return out;
}

IASTElement** MakeProgram()
{
	IASTElement** out = calloc(sizeof(Program), 1);
	Program_ctor(out);
	return out;
}

