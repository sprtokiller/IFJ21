#define PARSER_IMPL
#include "../include/parser.h"
#include "../include/Grammar.inl"
#include "syntax.h"
#include <malloc.h>

void Constructor(selfptr, FILE* file)
{
	self->program = MakeProgram();
	Vector_token_type_ctor(&self->stack);
	ExpressionAnalyzer_ctor(&self->exp);
	Scanner_ctor(&self->scan, file);
}
void Destructor(selfptr)
{
	Scanner_dtor(&self->scan);
	ExpressionAnalyzer_dtor(&self->exp);
	Vector_token_type_ctor(&self->stack);
	ppIASTElement_dtor(&self->program);
}

Error Start(selfptr)
{
	Error e = e_ok;
	*push_back_Vector_token_type(&self->stack) = tt_eof; //$ stack begin
	*push_back_Vector_token_type(&self->stack) = T0; //$ stack begin

	UNIQUE(token) t;
	ERR_CHECK(get_token(&self->scan, &t));

	token_type current_tt = tt_err;
	RetState rs = s_await;

	while (!empty_Vector_token_type(&self->stack))
	{
		current_tt = *back_Vector_token_type(&self->stack);
		if (current_tt >= T0 && current_tt <= T73) {
			pop_back_Vector_token_type(&self->stack);
			ERR_CHECK(LLTable(&self->stack, to_type(t.type), current_tt));
			continue;
		}
		if (current_tt == t.type || (current_tt == tt_type && is_type(t.type)) || current_tt == tt_expression)
		{
			if (current_tt == tt_expression && rs != s_await_e && rs != s_fcall)
			{
				unget_token(&self->scan, &t);
				ERR_CHECK(Execute(&self->exp, &self->scan));
				token_exp_ctor(&t, &self->exp.ast);
			}
			rs = (*self->program)->append(self->program, &t);
			if (rs == s_accept)
				break;

			DEBUG_ZERO(back_Vector_token_type(&self->stack));
			pop_back_Vector_token_type(&self->stack);
			if (current_tt == tt_eof)return e_ok;

			if (rs == s_fcall)
			{
				token p = {0};
				p.type = tt_left_parenthese;

				unget_token(&self->scan, &p);
				unget_token(&self->scan, &t);
				ERR_CHECK(Execute(&self->exp, &self->scan));
				token_exp_ctor(&t, &self->exp.ast);
				continue;
			}
			token_dtor(&t);

			if (rs == s_await_e)
			{
				Error ec = Execute(&self->exp, &self->scan);
				token_exp_ctor(&t, &self->exp.ast);
				t.ec = ec;
				continue;
			}
			ERR_CHECK(get_token(&self->scan, &t));
			if (rs == s_accept_fcall)
			{
				pop_back_Vector_token_type(&self->stack); // 
				pop_back_Vector_token_type(&self->stack); // )
			}

			continue;
		}
		return e_invalid_syntax;
	}
	(*self->program)->print(self->program);
	return e;
}
