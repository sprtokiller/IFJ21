
#define T(n) (token_type)(T0+n)
#define PUSH  *push_back_Vector_token_type(stack) = 

typedef Error(*Table)(Vector(token_type)* stack, token_type input_tt);

Error require_f(Vector(token_type)* stack, token_type top_tt)
{
	switch ((int)top_tt)
	{
	case T(0):
		PUSH T(1);
		PUSH tt_string_literal;
		PUSH tt_require;
		break;
	default:
		return e_invalid_syntax;
	}
	return e_ok;
}
Error identifier_f(Vector(token_type)* stack, token_type top_tt)
{
	switch ((int)top_tt)
	{
	case T(1):
		PUSH T(1);
		PUSH T(4);
		PUSH tt_identifier;
		break;
	case T(5):
		PUSH T(11);
		PUSH T(10);

	case T(6):
	case T(12):
	case T(21):
	case T(22):
	case T(29):
	case T(33):
	case T(36):
	case T(55):
	case T(58):
	case T(61):
	case T(62):
	case T(65):
	case T(66):
	case T(68):
	case T(70):
		break;

	case T(7):
		PUSH T(7);
		PUSH T(13);
		break;
	case T(10):
		PUSH tt_type;
		PUSH tt_colon;
		PUSH tt_identifier;
		break;
	case T(13):
		PUSH T(24);
		PUSH tt_identifier;
		break;
	case T(26):
		PUSH T(26);
		PUSH T(37);
		break;
	case T(31):
		PUSH T(31);
		PUSH T(44);
		break;
	case T(37):
		PUSH T(53);
		PUSH tt_identifier;
		break;
	case T(44):
		PUSH T(56);
		PUSH tt_identifier;
		break;
	default:
		return e_invalid_syntax;
	}
	return e_ok;
}

Error LLTable(Vector(token_type)* stack, token_type input_tt, token_type top_tt)
{
	switch (input_tt)
	{
	case tt_require:return require_f(stack, top_tt);
	case tt_identifier: return identifier_f(stack, top_tt);

	default:
		return e_invalid_syntax;
	}
}








#undef T
#undef PUSH