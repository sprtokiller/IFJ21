#define NODE_IMPL
#include "ast.h"
#include <stdio.h>


void Constructor(selfptr)
{
	self->valid = false;
	self->expression = false;
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
		printf("(%lld)", self->core.ival); return;
	case v_flt:
		printf("(%.9lf)", self->core.dval); return;
	case v_str:
		printf("(%s)", c_str(&self->core.sval)); return;
	default:
		break;
	};

	switch (self->core.type)
	{
	case tt_left_parenthese:
		print = "("; break;
	case tt_right_parenthese:
		print = ")"; break;
	case tt_u_minus: case tt_subtract:
		print = "-"; break;
	case tt_u_plus: case tt_add:
		print = "+"; break;
	case tt_multiply:
		print = "*"; break;
	case tt_divide:
		print = "/"; break;
	case tt_modulo:
		print = "%"; break;
	case tt_int_divide:
		print = "//"; break;
	case tt_power:
		print = "^"; break;
	case tt_comma:
		print = ","; break;
	case tt_not:
		print = "not"; break;
	case tt_or:
		print = "or"; break;
	case tt_and:
		print = "and"; break;
		//TODO: add more
	default:
		break;
	}
	printf("(%s)", print);
}

#include <malloc.h>
#include <string.h>

const char* subtree_prefix = "  |";
const char* space_prefix = "   ";

char* make_prefix(char* prefix, const char* suffix) {
	char* result = (char*)malloc(strlen(prefix) + strlen(suffix) + 1);
	strcpy(result, prefix);
	result = strcat(result, suffix);
	return result;
}

typedef enum { none, right , left}direction_t;


void print_subtree(const selfptr, char* prefix, direction_t from) {
	if (self != NULL) {
		char* current_subtree_prefix = make_prefix(prefix, subtree_prefix);
		char* current_space_prefix = make_prefix(prefix, space_prefix);

		if (from == left) {
			printf("%s\n", current_subtree_prefix);
		}

		print_subtree(
			self->right,
			from == left ? current_subtree_prefix : current_space_prefix, right);

		printf("%s  +-", prefix);
		PrintNodeVal(self);
		printf("\n");

		print_subtree(
			self->left,
			from == right ? current_subtree_prefix : current_space_prefix, left);

		if (from == right) {
			printf("%s\n", current_subtree_prefix);
		}

		free(current_space_prefix);
		free(current_subtree_prefix);
	}
}

void print_tree(const selfptr) {
	printf("Binary tree structure:\n");
	printf("\n");
	print_subtree(self, "", none);
	printf("\n");
}