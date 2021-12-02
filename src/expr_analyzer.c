#define EXPRA_IMPL
#include "expr_analyzer.h"
#include "scanner.h"
#include "semantic.h"
#include <inttypes.h>
#include <string.h>

typedef enum {
	S,  // shift           (<)
	R,  // reduce          (>)
	E,  // equal           (=)
	X   // nothing - error ( )
}ExprAction;

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
	case tt_identifier:case tt_nil:case tt_double_literal:case tt_string_literal:case tt_int_literal:case tt_true: case tt_false: case tt_fcall:
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
	return (index == 0 || (index > 1 && index < 5) || (index > 6 && index < 10) || index == 12);
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
			{
				token* t = &back_Vector_Node(&self->ast)->core;
				if (t->type != tt_err)
					unget_token(scanner, t);
				return e_invalid_syntax;
			}

			v(last_nt)->left = v(back_Vector_ptrdiff_t(&tree));
			token* t = &back_Vector_Node(&self->ast)->core;
			if (t->type != tt_err)
				unget_token(scanner, t);
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
			if (self->ast.end_[-2].core.type == tt_left_parenthese && !is_func) //empty closures
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
				if (!expr)return e_invalid_syntax;

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


static bool numeric(token_type tt)
{
	return tt == tt_number || tt == tt_integer;
}

token_type GetNodeType(Node* node, SemanticAnalyzer* analyzer, bool simple, Error* err);



Error MatchFunctionIns(Node* node, SemanticAnalyzer* analyzer, Span_token_type subsp)
{
	Error e = e_ok;
	token_type l = GetNodeType(node->left, analyzer, false, &e);
	token_type r = GetNodeType(node->right, analyzer, false, &e);
	if (e)return e;

	if (l == tt_eof)return empty_Span_token_type(&subsp) ? e_ok : e_count;
	if (node->core.type == tt_fcall && l == tt_comma)return MatchFunctionIns(node->left, analyzer, subsp);

	if (!FitsType(*subsp.begin, l))return e_count;
	if (r == tt_eof && (subsp.begin + 1) == subsp.end)return e_ok;
	if (l != *subsp.begin && l != tt_comma)node->left->result = tt_int_convert;
	if (r == tt_comma)
		return MatchFunctionIns(node->right, analyzer, (Span_token_type) { subsp.begin + 1, subsp.end });
	if (!FitsType(*(subsp.begin + 1), r))return e_count;
	if (r != *(subsp.begin + 1))node->right->result = tt_int_convert;

	return e_ok;
}

bool AnyNil(token_type a, token_type b)
{
	return a == tt_nil || b == tt_nil;
}

token_type GetNodeType(Node* node, SemanticAnalyzer* analyzer, bool simple, Error* err)
{
	if (!node)return tt_eof;
	token_type r = tt_eof, l = tt_eof;
	if (node->right)r = GetNodeType(node->right, analyzer, false, err);
	if (node->left)l = GetNodeType(node->left, analyzer, false, err);
	if (r == tt_err || l == tt_err)return tt_err;

	token_type res = literal_type(node->core.type);
	if (res)return res;

	switch (node->core.type)
	{
	case tt_identifier:
	{
		Variable* x = SA_FindVariable(analyzer, c_str(&node->core.sval));
		if (!x) {
			*err = e_redefinition;
			return tt_err;
		}
		token_dtor(&node->core);
		node->core.type = tt_internal_variable; //restate with pseudoname
		node->core.expression = x->asm_name;

		if (!x->has_value)
			return node->result = tt_nil;
		return node->result = x->type;
	}
	case tt_add:
	case tt_subtract:
	case tt_multiply:
		if (AnyNil(r, l)) {
			*err = e_RTnil;
			return tt_err;
		}
		if (!numeric(r) || !numeric(l))
		{
			*err = e_type;
			return tt_err;
		}
		if (l != r)
		{
			if (l == tt_number)node->right->result = tt_int_convert;
			if (r == tt_number)node->left->result = tt_int_convert;
		}
		return node->result = l;
	case tt_power:
		if (AnyNil(r, l)) {
			*err = e_RTnil;
			return tt_err;
		}
		if (r != tt_integer || !numeric(l))
		{
			*err = e_type;
			return tt_err;
		}
		if (l == tt_integer)
			node->result = node->left->result = tt_int_convert;
		return node->result = l;
	case tt_modulo:
	case tt_int_divide:
		if (AnyNil(r, l)) {
			*err = e_RTnil;
			return tt_err;
		}
		if (r != tt_integer || l != tt_integer)
		{
			*err = e_type;
			return tt_err;
		}
		if ((node->right->core.type == tt_int_literal) && node->right->core.ival == 0)
		{
			*err = e_RTzero;
			return tt_err;
		}
		return node->result = tt_integer;
	case tt_divide:
		if (AnyNil(r, l)) {
			*err = e_RTnil;
			return tt_err;
		}
		if (!numeric(r) || !numeric(l))
		{
			*err = e_type;
			return tt_err;
		}
		if ((node->right->core.type == tt_int_literal ||
			node->right->core.type == tt_double_literal) && node->right->core.ival == 0)
		{
			*err = e_RTzero;
			return tt_err;
		}
		if (l == tt_integer)
			node->left->result = tt_int_convert;
		if (r == tt_integer)
			node->right->result = tt_int_convert;
		return node->result = tt_number;
	case tt_g:
	case tt_ge:
	case tt_l:
	case tt_le:
		if (AnyNil(r, l)) {
			*err = e_RTnil;
			return tt_err;
		}
		if (!FitsType(r, l) && !FitsType(l, r))
		{
			*err = e_type;
			return tt_err;
		}
		if (l != r)
		{
			if (l == tt_number)node->right->result = tt_int_convert;
			if (r == tt_number)node->left->result = tt_int_convert;
		}
		return node->result = tt_boolean;
	case tt_ne:
	case tt_ee:
		if (!FitsType(r, l) && !FitsType(l, r) && (r != tt_nil && l != tt_nil))
		{
			*err = e_type;
			return tt_err;
		}
		if (l != r && (r != tt_nil && l != tt_nil))
		{
			if (l == tt_number)node->right->result = tt_int_convert;
			if (r == tt_number)node->left->result = tt_int_convert;
		}
		return node->result = tt_boolean;
	case tt_length:
		if (l != tt_string) {
			*err = e_type;
			return tt_err;
		}
		return node->result = tt_integer;
	case tt_concatenate:
		if (l != tt_string || r != tt_string) {
			*err = e_type;
			return tt_err;
		}
		return node->result = tt_string;
	case tt_and:
	case tt_or:
		if (l != tt_boolean || r != tt_boolean) {
			*err = e_type;
			return tt_err;
		}
		return node->result = tt_boolean;
	case tt_not:
		if (l != tt_boolean) {
			*err = e_type;
			return tt_err;
		}
		return node->result = tt_boolean;
	case tt_u_minus:
		if (!numeric(l)) {
			*err = e_type;
			return tt_err;
		}
		return node->result = l;
	case tt_comma:
		return tt_comma;
	case tt_fcall:
	{
		FunctionDecl* fd = SA_FindFunction(analyzer, c_str(&node->core.sval));
		if (!fd) {
			*err = e_redefinition;
			return tt_err;
		}
		if (!IsWrite(c_str(&node->core.sval)))
			if ((*err = MatchFunctionIns(node, analyzer, fd->types)))
			{
				e_msg("Bad function call");
				return tt_err;
			}
		fd->called = true;
		if (fd->ret.end - fd->ret.begin == 1) return node->result = *fd->ret.begin;
		if (simple) return node->result = tt_fcall;
		*err = e_other;
		return tt_err;
	}
	default:
		*err = e_other;
		return tt_err;
	}
}
token_type GetExpType(Vector(Node)* ast, SemanticAnalyzer* analyzer, Error* err)
{
	return GetNodeType(ast->data_->left, analyzer, true, err);
}

size_t Argc(Node* self)
{
	size_t x = 0;
	if (self->core.type != tt_comma && self->core.type != tt_fcall)return 1;
	if (self->right)x += Argc(self->right);
	if (self->left)x += Argc(self->left);
	return x;
}
void GenerateNode(Node* self, String* to)
{
	if (self->right)GenerateNode(self->right, to);
	if (self->left)GenerateNode(self->left, to);

	switch (self->core.type)
	{
	case tt_internal_variable:
		append_str(to, "PUSHS ");
		append_str(to, (const char*)self->core.expression);
		push_back_str(to, '\n');
		break;
	case tt_int_literal:
	{
		char c[128];
		sprintf(c, "%"PRId64"\n", self->core.ival);
		append_str(to, "PUSHS int@");
		append_str(to, c);
		break;
	}
	case tt_double_literal:
	{
		char c[128];
		sprintf(c, "%a\n", self->core.dval);
		append_str(to, "PUSHS float@");
		append_str(to, c);
		break;
	}
	case tt_string_literal:
		append_str(to, "PUSHS string@");
		append_str(to, c_str(&self->core.sval));
		push_back_str(to, '\n');
		break;
	case tt_power:
		append_str(to, "CALL $$_builtin_pow\n");
		break;
	case tt_length:
		append_str(to, "POPS GF@__XTMP_STR\n"
			"STRLEN GF@__XTMP_STRLEN GF@__XTMP_STR\n"
			"PUSHS GF@__XTMP_STRLEN\n");
		break;
	case tt_multiply:
		append_str(to, "MULS\n");
		break;
	case tt_divide:
		append_str(to, "DIVS\n");
		break;
	case tt_int_divide:
		append_str(to, "IDIVS\n");
		break;
	case tt_add:
		append_str(to, "ADDS\n");
		break;
	case tt_subtract:
		append_str(to, "SUBS\n");
		break;
	case tt_and:
		append_str(to, "ANDS\n");
		break;
	case tt_or:
		append_str(to, "ORS\n");
		break;
	case tt_not:
		append_str(to, "NOTS\n");
		break;
	case tt_u_minus:
		if (self->left->result == tt_integer)
		{
			append_str(to, "PUSHS int@0\n"
				"SUBS\n"); break;
		}
		if (self->left->result == tt_number
			|| self->left->result == tt_int_convert)
		{
			append_str(to, "PUSHS float@0\n"
				"SUBS\n"); break;
		}
		break;
	case tt_modulo:
		append_str(to, "CREATEFRAME\n"
			"PUSHFRAME\n"
			"DEFVAR LF@a\n"
			"POPS LF@a\n"
			"DEFVAR LF@m\n"
			"POPS LF@in\n"
			"DEFVAR LF@res\n"
			"IDIV LF@res LF@a LF@m\n"
			"MUL LF@res LF@a LF@m\n"
			"SUB LF@a LF@a LF@res\n"
			"PUSHS LF@a\n"
			"POPFRAME\n");
		break;

	case tt_g:
		append_str(to, "GTS\n");
		break;
	case tt_l:
		append_str(to, "LTS\n");
		break;
	case tt_le:
		append_str(to, "GTS\n""NOTS\n");
		break;
	case tt_ge:
		append_str(to, "LTS\n""NOTS\n");
		break;
	case tt_ee:
		append_str(to, "EQS\n");
		break;
	case tt_ne:
		append_str(to, "EQS\n""NOTS\n");
		break;
	case tt_fcall:
		if (!strcmp(c_str(&self->core.sval), "write"))
		{
			char c[128];
			sprintf(c, "%zu", Argc(self));
			append_str(to, "PUSHS int@");
			append_str(to, c);
			push_back_str(to, '\n');
		}
		append_str(to, "CALL $$");
		append_str(to, c_str(&self->core.sval));
		push_back_str(to, '\n');
	default:
		break;
	}

	if (self->result == tt_int_convert)
		append_str(to, "INT2FLOATS\n");

}
void GenerateExpression(Vector(Node)* self, String* to)
{
	GenerateNode(self->data_->left, to);
}
