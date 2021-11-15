#define NODE_IMPL
#include "ast.h"
#include <stdio.h>


void Constructor(selfptr)
{
	self->valid = 0;
	self->left = self->right = NULL;
}
void Destructor(selfptr)
{
	if (self->valid)
		token_dtor(&self->core);
}

token* Node_emplace(selfptr)
{
	Destructor(self);
	self->valid = true;
	return &self->core;
}

void PrintNodeVal(const selfptr)
{
	const char* print = "";
	switch (self->core.var)
	{
	case v_int:
		printf("%d", self->core.ival); return;
	case v_flt:
		printf("%.9lf", self->core.dval); return;
	case v_str:
		printf("%s", c_str(&self->core.sval)); return;
	default:
		break;
	};

	switch (self->core.type)
	{
	case tt_left_parenthese:
		print = "(";
	case tt_right_parenthese:
		print = ")";
	case tt_u_minus: case tt_subtract:
		print = "-";
	case tt_u_plus: case tt_add:
		print = "+";
	case tt_multiply:
		print = "*";
	case tt_divide:
		print = "/";
	case tt_modulo:
		print = "%";
	case tt_int_divide:
		print = "//";
		//TODO: add more
	default:
		break;
	}
	printf("%s", print);
}
void printBT(const selfptr, const char* prefix, bool isLeft)
{
	if (!self)return;
	printf("%s", isLeft ? "+---" : "+---");
	PrintNodeVal(self);
	// print the value of the node
	putchar('\n');

	// enter the next tree level - left and right branch
	printBT(self->left, isLeft ? "|   " : "    ", true);
	printBT(self->right, isLeft ? "|   " : "    ", false);
}

void print_tree(const selfptr)
{
	printBT(self, L"", false);
}