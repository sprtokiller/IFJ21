
#define CASE(name) Error name##_f(Vector(token_type)* stack, token_type input_tt)
#define T(n) (token_type)(T0+n)

typedef Error(*Table)(Vector(token_type)* stack, token_type input_tt);

Error T0_f(Vector(token_type)* stack, token_type input_tt)
{
	switch (input_tt)
	{
	case tt_require:
		*push_back_Vector_token_type(stack) = T(1);
		*push_back_Vector_token_type(stack) = tt_string_literal;
		*push_back_Vector_token_type(stack) = tt_require;
	default:
		return e_invalid_syntax;
	}
}

Table LLRows[] = {
	T0_f
};

#undef T