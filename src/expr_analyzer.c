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


PNode* LastNT(Vector(PNode)* tree)
{
	PNode* rbeg = back_Vector_PNode(tree);
	PNode* rend = front_Vector_PNode(tree) - 1;

	while (rbeg-- != rend) {
		if (!(*rbeg)->expression)return rbeg;
	}
	return NULL; //it should never go here
}

Error Execute(ExpressionAnalyzer* self, Scanner* scanner)
{
	Error e = e_ok;

	unique_vector(PNode) tree; // unsafe
	Vector_PNode_ctor(&tree);

	Node* end = push_back_Vector_Node(&self->ast);
	Node_ctor(end);
	token_ctor(Node_emplace(end), tt_err, NULL); //push $
	*push_back_Vector_PNode(&tree) = end;

	Node* node = push_back_Vector_Node(&self->ast);
	Node_ctor(node);
	ERR_CHECK(_get_token(scanner, Node_emplace(node)));
	*push_back_Vector_PNode(&tree) = node;


	while (e == e_ok)
	{
		PNode* last_nt = LastNT(&tree); //node to kill

		int input_i = table_column(&node->core);
		int last_i = table_column(&(*last_nt)->core);

		ExprAction rule = precedence_table[last_i][input_i];

		switch (rule)
		{
		case S:
			node = push_back_Vector_Node(&self->ast);
			Node_ctor(node);
			ERR_CHECK(_get_token(scanner, Node_emplace(node)));
			*push_back_Vector_PNode(&tree) = node;
			continue;
		case E:
		{
			(*last_nt)->expression = true;
			PNode* br_o = LastNT(&tree);
			if ((*br_o)->core.type != tt_left_parenthese)
				e = e_invalid_syntax;
			erase_Vector_PNode(&tree, br_o);
			erase_Vector_Node(&self->ast, *br_o);//remove parens from stack
			break;
		}
		case X:
		case R:
			(*last_nt)->expression = true;
			if (last_i == 1) {//unary
				(*last_nt)->left = last_nt[1];
				pop_back_Vector_PNode(&tree);
			}
			break;
		default:
			break;
		}

	}
	return e;
}
