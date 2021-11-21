
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
		break;
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

Error function_f(Vector(token_type)* stack, token_type top_tt)
{
	switch ((int)top_tt)
	{
	case T(1):
		PUSH T(1);
		PUSH T(2);
		break;
	case T(2):
		PUSH tt_end;
		PUSH T(7);
		PUSH T(6);
		PUSH tt_right_parenthese;
		PUSH T(5);
		PUSH tt_left_parenthese;
		PUSH tt_identifier;
		PUSH tt_function;
		break;
	case T(8):
		PUSH T(21);
		PUSH tt_right_parenthese;
		PUSH T(20);
		PUSH tt_left_parenthese;
		PUSH tt_function;
		break;
	case T(21):
	case T(22):
	case T(33):
		break;
	case T(25):
		PUSH T(6);
		PUSH tt_right_parenthese;
		PUSH T(20);
		PUSH tt_left_parenthese;
		PUSH tt_function;
		break;
	case T(54):
		PUSH T(61);
		PUSH tt_right_parenthese;
		PUSH T(20);
		PUSH tt_left_parenthese;
		PUSH tt_function;
		break;
	case T(57):
		PUSH T(65);
		PUSH tt_right_parenthese;
		PUSH T(20);
		PUSH tt_left_parenthese;
		PUSH tt_function;
		break;
	default:
		return e_invalid_syntax;
	}
	return e_ok;
}

Error left_parenthese_f(Vector(token_type)* stack, token_type top_tt)
{
	switch ((int)top_tt)
	{
	case T(4):
		PUSH T(9);
		PUSH tt_left_parenthese;
		break;
	case T(24):
		PUSH T(34);
		break;
	case T(34):
		PUSH T(51);
		PUSH tt_left_parenthese;
		break;
	case T(53):
		PUSH T(59);
		break;
	case T(56):
		PUSH T(63);
		break;
	case T(59):
		PUSH T(67);
		PUSH tt_left_parenthese;
		break;
	case T(63):
		PUSH T(69);
		PUSH tt_left_parenthese;
		break;
	default:
		return e_invalid_syntax;
	}
	return e_ok;
}

Error right_parenthese_f(Vector(token_type)* stack, token_type top_tt)
{
	switch ((int)top_tt)
	{
	case T(5):
	case T(11):
	case T(20):
	case T(23):
	case T(32):
		break;
	case T(9):
	case T(51):
	case T(67):
	case T(69):
		PUSH tt_right_parenthese;
		break;
	default:
		return e_invalid_syntax;
	}
	return e_ok;
}

Error end_f(Vector(token_type)* stack, token_type top_tt) 
{
	switch ((int)top_tt)
	{
	case T(6):
	case T(7):
	case T(12):
	case T(26):
	case T(27):
	case T(28):
	case T(29):
	case T(36):
	case T(55):
	case T(61):
	case T(62):
	case T(68):
		break;
	default:
		return e_invalid_syntax;
	}
	return e_ok;
}

Error colon_f(Vector(token_type)* stack, token_type top_tt) 
{
	switch ((int)top_tt)
	{
	case T(6):
		PUSH T(12);
		PUSH tt_type;
		PUSH tt_colon;
		break;
	case T(21):
		PUSH T(33);
		PUSH tt_type;
		PUSH tt_colon;
		break;
	case T(61):
		PUSH T(68);
		PUSH tt_type;
		PUSH tt_colon;
		break;
	case T(65):
		PUSH T(70);
		PUSH tt_type;
		PUSH tt_colon;
		break;
	default:
		return e_invalid_syntax;
	}
	return e_ok;
}

Error type_f(Vector(token_type)* stack, token_type top_tt) 
{
	switch ((int)top_tt)
	{
	case T(8):
		PUSH T(22);
		PUSH tt_type;
		break;
	case T(20):
		PUSH T(32);
		PUSH tt_type;
		break;
	case T(25):
		PUSH T(36);
		PUSH tt_type;
		break;
	case T(54):
		PUSH T(62);
		PUSH tt_type;
		break;
	case T(57):
		PUSH T(66);
		PUSH tt_type;
		break;
	default:
		return e_invalid_syntax;
	}
	return e_ok;
}

Error comma_f(Vector(token_type)* stack, token_type top_tt) 
{
	switch ((int)top_tt)
	{
	case T(11):
		PUSH T(11);
		PUSH T(10);
		PUSH tt_comma;
		break;
	case T(12):
		PUSH T(12);
		PUSH tt_type;
		PUSH tt_comma;
		break;
	case T(23):
		PUSH T(23);
		PUSH tt_expression;
		PUSH tt_comma;
		break;
	case T(24):
		PUSH T(35);
		break;
	case T(29):
		PUSH T(29);
		PUSH tt_expression;
		PUSH tt_comma;
		break;
	case T(32):
		PUSH T(32);
		PUSH tt_type;
		PUSH tt_comma;
		break;
	case T(33):
		PUSH T(33);
		PUSH tt_type;
		PUSH tt_comma;
		break;
	case T(35):
		PUSH T(29);
		PUSH tt_expression;
		PUSH tt_assign;
		PUSH T(52);
		break;
	case T(52):
		PUSH T(52);
		PUSH tt_identifier;
		PUSH tt_comma;
		break;
	case T(53):
		PUSH T(60);
		break;
	case T(55):
		PUSH T(55);
		PUSH tt_expression;
		PUSH tt_comma;
		break;
	case T(56):
		PUSH T(64);
		break;
	case T(57):
		PUSH T(58);
		PUSH tt_expression;
		PUSH tt_comma;
		break;
	case T(60):
		PUSH T(55);
		PUSH tt_expression;
		PUSH tt_assign;
		PUSH T(52);
		break;
	case T(64):
		PUSH T(58);
		PUSH tt_expression;
		PUSH tt_assign;
		PUSH T(52);
		break;
	case T(68):
		PUSH T(68);
		PUSH tt_type;
		PUSH tt_comma;
		break;
	case T(70):
		PUSH T(70);
		PUSH tt_type;
		PUSH tt_comma;
		break;
	default:
		return e_invalid_syntax;
	}
	return e_ok;
}

Error global_f(Vector(token_type)* stack, token_type top_tt)
{
	switch ((int)top_tt)
	{
	case T(1):
		PUSH T(1);
		PUSH T(3);
		break;
	case T(3):
		PUSH T(8);
		PUSH tt_colon;
		PUSH tt_identifier;
		PUSH tt_global;
		break;
	case T(21):
	case T(22):
	case T(33):
		break;
	default:
		return e_invalid_syntax;
	}
	return e_ok;
}

Error assign_f(Vector(token_type)* stack, token_type top_tt) 
{
	switch ((int)top_tt)
	{
	case T(22):
		PUSH tt_expression;
		PUSH tt_assign;
		break;
	case T(24):
		PUSH T(35);
		break;
	case T(35):
		PUSH T(29);
		PUSH tt_expression;
		PUSH tt_assign;
		PUSH T(52);
		break;
	case T(36):
		PUSH tt_expression;
		PUSH tt_assign;
		break;
	case T(52):
		break;
	case T(53):
		PUSH T(60);
		break;
	case T(56):
		PUSH T(64);
		break;
	case T(60):
		PUSH T(55);
		PUSH tt_expression;
		PUSH tt_assign;
		PUSH T(52);
		break;
	case T(62):
		PUSH tt_expression;
		PUSH tt_assign;
		break;
	case T(64):
		PUSH T(58);
		PUSH tt_expression;
		PUSH tt_assign;
		PUSH T(52);
		break;
	case T(66):
		PUSH tt_expression;
		PUSH tt_assign;
		break;
	default:
		return e_invalid_syntax;
	}
	return e_ok;
}

Error expression_f(Vector(token_type)* stack, token_type top_tt) 
{
	switch ((int)top_tt)
	{
	case T(9):
	case T(51):
	case T(67):
	case T(69):
		PUSH T(23);
		PUSH tt_expression;
		break;
	case T(30):
		PUSH tt_expression;
		break;
	default:
		return e_invalid_syntax;
	}
	return e_ok;
}


Error local_f(Vector(token_type)* stack, token_type top_tt)
{
	switch ((int)top_tt)
	{
	case T(6):
	case T(17):
	case T(29):
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
		PUSH T(14);
		break;
	case T(14):
		PUSH T(25);
		PUSH tt_colon;
		PUSH tt_identifier;
		PUSH tt_local;
		break;
	case T(26):
		PUSH T(26);
		PUSH T(38);
		break;
	case T(31):
		PUSH T(31);
		PUSH T(45);
		break;
	case T(38):
		PUSH T(54);
		PUSH tt_colon;
		PUSH tt_identifier;
		PUSH tt_local;
		break;
	case T(45):
		PUSH T(57);
		PUSH tt_colon;
		PUSH tt_identifier;
		PUSH tt_local;
		break;
	default:
		return e_invalid_syntax;
	}
	return e_ok;
}

Error if_f(Vector(token_type)* stack, token_type top_tt)
{
	switch ((int)top_tt)
	{
	case T(6):
	case T(12):
	case T(29):
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
		PUSH T(15);
		break;
	case T(15):
		PUSH tt_end;
		PUSH T(28);
		PUSH T(27);
		PUSH T(26);
		PUSH tt_then;
		PUSH tt_expression;
		PUSH tt_if;
		break;
	case T(26):
		PUSH T(26);
		PUSH T(39);
		break;
	case T(31):
		PUSH T(31);
		PUSH T(46);
		break;
	case T(39):
	case T(46):
		PUSH tt_end;
		PUSH T(28);
		PUSH T(27);
		PUSH T(26);
		PUSH tt_then;
		PUSH tt_expression;
		PUSH tt_if;
		break;
	default:
		return e_invalid_syntax;
	}
	return e_ok;
}

Error then_f(Vector(token_type)* stack, token_type top_tt) 
{
	switch ((int)top_tt)
	{
	default:
		return e_invalid_syntax;
	}
	return e_ok;
}

Error elseif_f(Vector(token_type)* stack, token_type top_tt) 
{
	switch ((int)top_tt)
	{
	case T(26):
	case T(55):
	case T(61):
	case T(62):
	case T(68):
		break;
	case T(27):
		PUSH T(7);
		PUSH tt_else;
		break;
	default:
		return e_invalid_syntax;
	}
	return e_ok;
}

Error while_f(Vector(token_type)* stack, token_type top_tt)
{
	switch ((int)top_tt)
	{
	case T(6):
	case T(12):
	case T(29):
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
		PUSH T(16);
		break;
	case T(16):
		PUSH tt_end;
		PUSH T(7);
		PUSH tt_do;
		PUSH tt_expression;
		PUSH tt_while;
		break;
	case T(26):
		PUSH T(26);
		PUSH T(40);
		break;
	case T(31):
		PUSH T(31);
		PUSH T(47);
		break;
	case T(40):
	case T(47):
		PUSH tt_end;
		PUSH T(7);
		PUSH tt_do;
		PUSH tt_expression;
		PUSH tt_while;
		break;
	default:
		return e_invalid_syntax;
	}
	return e_ok;
}

Error do_f(Vector(token_type)* stack, token_type top_tt)
{
	switch ((int)top_tt)
	{
	case T(30):
		break;
	default:
		return e_invalid_syntax;
	}
	return e_ok;
}

Error return_f(Vector(token_type)* stack, token_type top_tt)
{
	switch ((int)top_tt)
	{
	case T(6):
	case T(29):
	case T(12):
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
		PUSH T(17);
		break;
	case T(17):
		PUSH T(29);
		PUSH tt_return;
		break;
	case T(26):
		PUSH T(26);
		PUSH T(41);
		break;
	case T(31):
		PUSH T(31);
		PUSH T(48);
		break;
	case T(41):
		PUSH T(55);
		PUSH tt_return;
		break;
	case T(48):
		PUSH T(58);
		PUSH tt_return;
		break;
	default:
		return e_invalid_syntax;
	}
	return e_ok;
}

Error for_f(Vector(token_type)* stack, token_type top_tt)
{
	switch ((int)top_tt)
	{
	case T(6):
	case T(12):
	case T(29):
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
		PUSH T(18);
		break;
	case T(18):
	case T(42):
	case T(49):
		PUSH tt_end;
		PUSH T(7);
		PUSH tt_do;
		PUSH T(30);
		PUSH tt_expression;
		PUSH tt_comma;
		PUSH tt_expression;
		PUSH tt_assign;
		PUSH tt_identifier;
		PUSH tt_for;
		break;
	case T(26):
		PUSH T(26);
		PUSH T(42);
		break;
	case T(31):
		PUSH T(31);
		PUSH T(49);
		break;
	default:
		return e_invalid_syntax;
	}
	return e_ok;
}

Error repeat_f(Vector(token_type)* stack, token_type top_tt)
{
	switch ((int)top_tt)
	{
	case T(6):
	case T(12):
	case T(28):
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
		PUSH T(19);
		break;
	case T(19):
	case T(43):
	case T(50):
		PUSH tt_expression;
		PUSH tt_until;
		PUSH T(31);
		PUSH tt_repeat;
		break;
	case T(31):
		PUSH T(31);
		PUSH T(50);
		break;
	default:
		return e_invalid_syntax;
	}
	return e_ok;
}

Error until_f(Vector(token_type)* stack, token_type top_tt) 
{
	switch ((int)top_tt)
	{
	case T(31):
	case T(58):
	case T(65):
	case T(66):
	case T(70):
		break;
	default:
		return e_invalid_syntax;
	}
	return e_ok;
}

Error eof_f(Vector(token_type)* stack, token_type top_tt)
{
	switch ((int)top_tt)
	{
	case T(1):
	case T(21):
	case T(22):
	case T(33):
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
	case tt_require:			return require_f(stack, top_tt);
	case tt_identifier:			return identifier_f(stack, top_tt);
	case tt_function:			return function_f(stack, top_tt);
	case tt_left_parenthese:	return left_parenthese_f(stack, top_tt);
	case tt_right_parenthese:	return right_parenthese_f(stack, top_tt);
	case tt_end:				return end_f(stack, top_tt);
	case tt_colon:				return colon_f(stack, top_tt);
	case tt_type:				return type_f(stack, top_tt);
	case tt_comma:				return comma_f(stack, top_tt);
	case tt_global:				return global_f(stack, top_tt);
	case tt_assign:				return assign_f(stack, top_tt);
	case tt_expression:			return expression_f(stack, top_tt);
	case tt_local:				return local_f(stack, top_tt);
	case tt_if:					return if_f(stack, top_tt);
	case tt_then:				return then_f(stack, top_tt);
	case tt_elseif:				return elseif_f(stack, top_tt);
	case tt_while:				return while_f(stack, top_tt);
	case tt_do:					return do_f(stack, top_tt);
	case tt_return:				return return_f(stack, top_tt);
	case tt_for:				return for_f(stack, top_tt);
	case tt_repeat:				return repeat_f(stack, top_tt);
	case tt_until:				return until_f(stack, top_tt);
	case tt_eof:				return eof_f(stack, top_tt);
	default:
		return e_invalid_syntax;
	}
}

#undef T
#undef PUSH