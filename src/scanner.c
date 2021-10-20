#include "scanner.h"
#include <ctype.h>

typedef enum
{
	s_begin,
	s_int,
	s_minus,
	s_plus,
	s_float = 1 << 13,
	s_expf = 1 << 15,
	s_expf_pm = 1 << 14
}State;

void Scanner_ctor(Scanner* self, FILE* source)
{
	Vector_token_ctor(&self->tk_stream);
	self->source = source;
	self->line = 1;
	self->column = 0;
	self->sym = 0;
	self->prev_state = s_begin;
}
void Scanner_dtor(Scanner* self)
{
	Vector_token_dtor(&self->tk_stream);
}


int fetch_symbol(Scanner* self)
{
	int sym = fgetc(self->source);
	if (sym != EOF) {
		self->column++;
		if (sym == '\n') { self->line++; self->column = 0; }
	}
	return sym;
}
bool probably_op(uint32_t state)
{
	return state == s_int || (state & 7 << 13) > 0;
}

typedef struct
{
	uint32_t* state_to;
	State* state_from;
	int* sym_to;
	int* sym_from;
}sym_guard;
void sym_guard_dtor(sym_guard* self)
{
	*self->sym_to = *self->sym_from;
	*self->state_to = *self->state_from;
}

Error _get_token(Scanner* self, token* tk)
{
	unique_string xtoken;

	String_ctor(&xtoken, NULL);
	uint32_t tkstart_col = 0;
	uint32_t tkstart_line = 1;
	token_type predict = tt_eof;

	int sym = 0;
	State state = s_begin;

	UNIQUE(sym_guard) sg;
	sg.sym_to = &self->sym;
	sg.sym_from = &sym;
	sg.state_to = &self->prev_state;
	sg.state_from = &state;


	Error e = Ok;
	bool valid = false;

	if (self->sym) { sym = self->sym; goto inbound; }

	while ((sym = fetch_symbol(self)) != EOF)
	{
	inbound:

		switch (state)
		{
		case s_begin:
			switch (sym)
			{
			case '+':
			case '-':
				predict = sym > '+' ? tt_subtract : tt_add;
				state = sym > '+' ? s_minus : s_plus;
				if (probably_op(self->prev_state))goto simple_tk;
				push_back_str(&xtoken, (char)sym);
				tkstart_col = self->column;
				tkstart_line = self->line;
				continue;
			case '*':
				predict = tt_multiply;
				goto simple_tk;
			case '/':
				predict = tt_divide;
				goto simple_tk;
			case '%':
				predict = tt_modulo;
				goto simple_tk;
			case ' ':
			case '\n':
				continue;
			default:
				break;
			simple_tk:
				token_ctor(tk, predict, NULL);
				tk->line = self->line;
				tk->column = self->column;
				sym = 0;
				return e;
			}
			if (isdigit(sym))
			{
				state = s_int;
				push_back_str(&xtoken, (char)sym);
				tkstart_col = self->column;
				tkstart_line = self->line;
			}
			else
			{
				sym = 0;
				return e;
			}
			break;
		case s_int:
			if (isdigit(sym)) {
				push_back_str(&xtoken, (char)sym);
			}
			else if (sym == '.')
			{
				state = s_float;
				push_back_str(&xtoken, (char)sym);
			}
			else if (sym == 'E' || sym == 'e') {
				state = s_expf;
				push_back_str(&xtoken, (char)sym);
				valid = false;
			}
			else
			{
				predict = tt_int_literal;
				goto make_token;
			}
			break;
		case s_float:
			if (isdigit(sym))
			{
				push_back_str(&xtoken, (char)sym);
				valid = true;
			}
			else if (sym == 'E' || sym == 'e')
			{
				state = s_expf;
				push_back_str(&xtoken, (char)sym);
				valid = false;
			}
			else
			{
				predict = tt_double_literal;
				goto check_valid;
			}
			break;
		case s_minus:
		case s_plus:
			if (isdigit(sym))
			{
				state = s_int;
				push_back_str(&xtoken, (char)sym);
			}
			else {
				goto make_token;
			}
			break;
		case s_expf:
			if (!valid && (sym == '+' || sym == '-'))
			{
				state = s_expf_pm;
				push_back_str(&xtoken, (char)sym);
				break;
			}
			/* fall through */
		case s_expf_pm:
			if (isdigit(sym))
			{
				valid = true;
				push_back_str(&xtoken, (char)sym);
			}
			else
			{
				predict = tt_double_literal;
				goto check_valid;
			}
		default:
			break;
		check_valid:
			if (!valid)return e_invalid_token;
		make_token:
			token_ctor(tk, predict, &xtoken);
			tk->line = tkstart_line;
			tk->column = tkstart_col;
			return e;
		}
	}
	return e_eof;
}
