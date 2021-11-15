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
bool is_binary(int index)
{
	return (index == 0 || (index > 1 && index < 5) || index == 8 || index == 9 || index == 12);
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


ptrdiff_t* LastNT(Vector(ptrdiff_t)* tree, Node* st_beg)
{
	ptrdiff_t* rbeg = back_Vector_ptrdiff_t(tree);
	ptrdiff_t* rend = front_Vector_ptrdiff_t(tree) - 1;

	while (rbeg != rend) {
		if (!(rbeg[0] + st_beg)->expression)return rbeg;
		rbeg--;
	}
	return NULL; //it should never go here
}
ptrdiff_t* LastE(Vector(ptrdiff_t)* tree, Node* st_beg)
{
	ptrdiff_t* rbeg = back_Vector_ptrdiff_t(tree);
	ptrdiff_t* rend = front_Vector_ptrdiff_t(tree) - 1;

	while (rbeg != rend) {
		if ((rbeg[0] + st_beg)->expression)return rbeg;
		rbeg--;
	}
	return NULL; //it should never go here
}

#define v(a) ((*a) + self->ast.data_)
#define r(a) (a - self->ast.data_)

Error Execute(ExpressionAnalyzer* self, Scanner* scanner)
{
	Error e = e_ok;

	unique_vector(ptrdiff_t) tree; // unsafe
	Vector_ptrdiff_t_ctor(&tree);

	Node* end = push_back_Vector_Node(&self->ast);
	Node_ctor(end);
	token_ctor(Node_emplace(end), tt_err, NULL); //push $
	*push_back_Vector_ptrdiff_t(&tree) = r(end);

	Node* node = push_back_Vector_Node(&self->ast);
	Node_ctor(node);
	ERR_CHECK(_get_token(scanner, Node_emplace(node)));


	while (e == e_ok)
	{
		ptrdiff_t* last_nt = LastNT(&tree, self->ast.data_); //node to kill

		int input_i = table_column(&node->core);
		int last_i = table_column(&v(last_nt)->core);

		ExprAction rule = precedence_table[last_i][input_i];

		if (input_i == 10 && last_i == 10)
		{
			if (size_Vector_ptrdiff_t(&tree) != 2)
				return e_invalid_syntax;
			v(last_nt)->left = v(back_Vector_ptrdiff_t(&tree));
			pop_back_Vector_Node(&self->ast);
			return e_ok;
		}

		switch (rule)
		{
		case S:
			*push_back_Vector_ptrdiff_t(&tree) = r(node);
			node = push_back_Vector_Node(&self->ast);
			Node_ctor(node);
			ERR_CHECK(_get_token(scanner, Node_emplace(node)));
			continue;
		case E:
		{
			v(last_nt)->expression = true;
			erase_Vector_ptrdiff_t(&tree, last_nt);
			node = push_back_Vector_Node(&self->ast);
			Node_ctor(node);
			ERR_CHECK(_get_token(scanner, Node_emplace(node)));
			break;
		}
		case X:
			return e_invalid_syntax;
		case R:
			if (last_i == 1) {//unary
				v(last_nt)->left = v(LastE(&tree, self->ast.data_));
				pop_back_Vector_ptrdiff_t(&tree);
			}else if (is_binary(last_i)) {//binary
				v(last_nt)->right = v(LastE(&tree, self->ast.data_));
				pop_back_Vector_ptrdiff_t(&tree);
				ptrdiff_t* xleft = LastE(&tree, self->ast.data_);
				v(last_nt)->left = v(xleft);
				erase_Vector_ptrdiff_t(&tree, xleft);
			}
			v(last_nt)->expression = true;

			break;
		default:
			break;
		}

	}
	return e;
}
