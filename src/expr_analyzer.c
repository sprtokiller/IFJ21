#define EXPRA_IMPL
#include "expr_analyzer.h"
#include "scanner.h"

#define invalid_i -1

const ExprAction precedence_table[][13] =
{
	{S,R,R,R,R,S,R,R,R,R,R,S,R},
	{X,X,R,R,X,S,R,R,R,R,R,S,R},
	{S,S,R,R,X,S,R,R,R,R,R,S,R},
	{S,S,S,R,X,S,R,R,R,R,R,S,R},
	{X,X,X,X,S,S,R,R,X,X,R,S,R},
	{S,S,S,S,S,S,E,S,S,S,X,S,S},
	{R,X,R,R,S,X,R,R,R,R,R,X,X},
	{S,S,S,S,S,S,R,R,R,R,R,S,R},
	{S,S,S,S,S,S,R,S,R,R,R,S,R},
	{S,S,S,S,S,S,R,S,S,R,R,S,R},
	{S,S,S,S,S,S,X,S,S,S,S,S,X},
	{R,X,R,R,R,S,R,R,R,R,R,X,R},
	{S,S,S,S,S,S,R,S,S,S,X,S,S}
};

static int table_column(const token* t) {
	switch (t->type)
	{
	case tt_power:
		return 0;
	case tt_u_minus:case tt_not:case tt_length:
		return 1;
	case tt_divide:case tt_int_divide:case tt_modulo:case tt_multiply:
		return 2;
	case tt_add:case tt_subtract:
		return 3;
	case tt_concatenate:
		return 4;
	case tt_left_parenthese:
		return 5;
	case tt_right_parenthese:
		return 6;
	case tt_and:
		return 8;
	case tt_or:
		return 9;
	case tt_identifier:case tt_double_literal:case tt_string_literal:case tt_int_literal:case tt_true: case tt_false:
		return 11;
	case tt_comma:
		return 12;

	default:
		if (t->type >= tt_l && t->type <= tt_ne)
			return 7;
		break;
	}
	return 10;
}



void Constructor(ExpressionAnalyzer* self)
{
	self->function = false;
	Vector_Node_ctor(&self->ast);
}
void Destructor(ExpressionAnalyzer* self)
{
	Vector_Node_dtor(&self->ast);
}


Node* LastNT(ExpressionAnalyzer* self)
{
	Node* rbeg = back_Vector_Node(&self->ast);
	Node* rend = front_Vector_Node(&self->ast) - 1;

	while (rbeg-- != rend){
		if (!rbeg->expression)return rbeg;
	}
	return NULL; //it should never go here
}

Error Execute(ExpressionAnalyzer* self, Scanner* scanner)
{
	Error e = e_ok;
	Node* end = push_back_Vector_Node(&self->ast);
	Node_ctor(end);
	token_ctor(Node_emplace(end), tt_err, NULL); //push $

	Node* node = push_back_Vector_Node(&self->ast);
	Node_ctor(node);
	ERR_CHECK(_get_token(scanner, Node_emplace(node)));

	Node* last_e = NULL;


	while (e == e_ok)
	{
		Node* last_nt = LastNT(self); //node to kill

		int input_i = table_column(&node->core);
		int last_i = table_column(last_nt);

		ExprAction rule = precedence_table[last_i][input_i];

		switch (rule)
		{
		case S:
			node = push_back_Vector_Node(&self->ast);
			Node_ctor(node);
			ERR_CHECK(_get_token(scanner, Node_emplace(node)));
			continue;
		case E:

		case X:
		case R:
			if (last_i == 1) //unary
			{
				last_nt->left = last_e;
				last_e = last_nt;
			}
			if (last_i == 11)
			{

			}
		default:
			break;
		}

	}
	return e;
}
