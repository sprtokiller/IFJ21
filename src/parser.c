#define PARSER_IMPL
#include "../include/parser.h"
#include "../include/Grammar.inl"
#include "syntax.h"

void Constructor(selfptr, FILE* file)
{
	Vector_token_type_ctor(&self->stack);
	ExpressionAnalyzer_ctor(&self->exp);
	Scanner_ctor(&self->scan, file);
}
void Destructor(selfptr)
{
	Scanner_dtor(&self->scan);
	ExpressionAnalyzer_dtor(&self->exp);
	Vector_token_type_ctor(&self->stack);
} 

Error Start(selfptr)
{
	Error e = e_ok;
	*push_back_Vector_token_type(&self->stack) = tt_eof; //$ stack begin
	*push_back_Vector_token_type(&self->stack) = T0; //$ stack begin

	token t;
	ERR_CHECK(get_token(&self->scan, &t));
	IASTElement** x = NULL;

	token_type current_tt = tt_err;
	while (!empty_Vector_token_type(&self->stack))
	{
		current_tt = *back_Vector_token_type(&self->stack);
		if (current_tt >= T0 && current_tt <= T70) {
			pop_back_Vector_token_type(&self->stack);
			ERR_CHECK(LLTable(&self->stack, to_type(t.type), current_tt));
			continue;
		}
		if (current_tt == t.type || (current_tt == tt_type && is_type(t.type)) || current_tt == tt_expression)
		{
			if (!x)
				x = MakeStatement(t.type);
			else 
				if ((*x)->append(x, &t)) { (*x)->print(x); (*x)->dtor(x); free(x); x = NULL; }

			//1. make actions
			//2. semantics
			//3. pop from stack
			
			if (current_tt == tt_expression)
				Execute(&self->exp, &self->scan);

			DEBUG_ZERO(back_Vector_token_type(&self->stack));
			pop_back_Vector_token_type(&self->stack);
			if (current_tt == tt_eof)return e_ok;
			token_dtor(&t);
			ERR_CHECK(get_token(&self->scan, &t));
			continue;
		}
		return e_invalid_syntax;
	}

	return e;
}
