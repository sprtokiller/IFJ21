#define PARSER_IMPL
#include "../include/parser.h"
#include "../include/Grammar.inl"

void Constructor(selfptr)
{
	Vector_token_type_ctor(&self->stack);
	ExpressionAnalyzer_ctor(&self->exp);
	Scanner_ctor(&self->scan, stdin);
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
	*push_back_Vector_token_type(&self->stack) = tt_err; //$ stack begin
	*push_back_Vector_token_type(&self->stack) = T0; //$ stack begin
	token t;
	ERR_CHECK(get_token(&self->scan, &t));

	token_type current_tt = tt_err;
	while (!empty_Vector_token_type(&self->stack))
	{
		current_tt = *back_Vector_token_type(&self->stack);
		if (current_tt >= T0 && current_tt <= T68){
			LLRows[current_tt](&self->stack, t.type);
			continue;
		}
		if (current_tt == t.type)
		{
			
		}
	}

	return e;
}
