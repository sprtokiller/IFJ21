#define EXPRA_IMPL
#include "expr_analyzer.h"
#include "scanner.h"

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
	case tt_identifier:case tt_double_literal:case tt_string_literal:case tt_int_literal:case tt_true: case tt_false: case tt_fcall:
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

	unique_vector(ptrdiff_t) tree;
	Vector_ptrdiff_t_ctor(&tree);

	Node* end = push_back_Vector_Node(&self->ast);
	Node_ctor(end);
	token_ctor(Node_emplace(end), tt_err, NULL); //push $
	*push_back_Vector_ptrdiff_t(&tree) = r(end);

	Node* node = push_back_Vector_Node(&self->ast);
	Node_ctor(node);
	ERR_CHECK(get_token(scanner, Node_emplace(node)));


	while (e == e_ok)
	{
		ptrdiff_t* last_nt = LastNT(&tree, self->ast.data_); //node to kill

		int input_i = table_column(&node->core);
		int last_i = table_column(&v(last_nt)->core);

		if (input_i == 11 && node->core.type == tt_identifier && last_i == 11)// id id means its time to stop
		{
			unget_token(scanner, &node->core);
			input_i = 10;
			node->core.type = tt_err;
		}

		ExprAction rule = precedence_table[last_i][input_i];

		if (input_i == 10 && last_i == 10)//handle closure $$
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
			ERR_CHECK(get_token(scanner, Node_emplace(node)));
			continue;
		case E:
		{
			Node* id_last = v(last_nt - 1);
			Node* last_ = v(last_nt);
			bool is_func = id_last->core.type == tt_identifier;
			if (last_->core.type == tt_left_parenthese && !is_func)
				return e_invalid_syntax;

			if (is_func)
			{
				id_last->core.type = tt_fcall;
				if (v(back_Vector_ptrdiff_t(&tree))->core.type != tt_left_parenthese)
				{
					id_last->left = v(LastE(&tree, self->ast.data_));
					pop_back_Vector_ptrdiff_t(&tree);
				}
			}
			last_->expression = true;
			erase_Vector_ptrdiff_t(&tree, last_nt);

			node = push_back_Vector_Node(&self->ast);
			Node_ctor(node);
			ERR_CHECK(get_token(scanner, Node_emplace(node)));

			break;
		}

		case R:
			if (last_i == 1) {//unary
				ptrdiff_t* expr = LastE(&tree, self->ast.data_);
				if (!expr || expr < last_nt)
					return e_invalid_syntax;
				v(last_nt)->left = v(expr);
				pop_back_Vector_ptrdiff_t(&tree);
			}
			else if (is_binary(last_i)) {//binary
				ptrdiff_t* expr = LastE(&tree, self->ast.data_);

				if (!expr || expr < last_nt)
					return e_invalid_syntax;
				v(last_nt)->right = v(expr);

				pop_back_Vector_ptrdiff_t(&tree);
				expr = LastE(&tree, self->ast.data_);
				if(!expr)return e_invalid_syntax;

				v(last_nt)->left = v(expr);
				erase_Vector_ptrdiff_t(&tree, expr);
			}
			v(last_nt)->expression = true;

			break;
		default:
			if (node->core.type == tt_comma && v(last_nt)->core.type == tt_err) //comma can be a separator
			{
				unget_token(scanner, &node->core);
				node->core.type = tt_err;
				continue;
			}
			return e_invalid_syntax;
		}

	}
	return e;
}
