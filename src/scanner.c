#include "scanner.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef enum
{
	s_begin,
	s_int,
	s_minus,
	s_plus,
	s_string,
	s_cmp,
	s_eof,
	s_cat,
	s_div,
	s_ne,
	s_esc,
	s_commentary,
	s_commentary_ml,
	s_kw,
	s_id,

	s_float = 1 << 13,
	s_expf,
	s_expf_pm
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
int skip_comment(Scanner* self)
{
	int sym = 0;
	while (sym != EOF && sym != '\n')
		sym = fetch_symbol(self);
	return sym;
}
int skip_space(Scanner* self, int sym)
{
	while (sym != EOF && sym == ' ')
		sym = fetch_symbol(self);
	return sym;
}
int skip_comment_ml(Scanner* self)
{
	bool br = false;
	int sym = 0;
	while (sym != EOF)
	{
		sym = fetch_symbol(self);
		if (!br && sym == ']') { br = true; continue; }
		if (br && sym == ']') { return sym; }
		br = false;
	}
	return sym;
}
bool is_operand(uint32_t state)
{
	return state == s_int || (state & 1 << 13) > 0 || state == s_id;
}

inline bool x_bsearch(const char* str, size_t len, int el)
{
	int l = 0;
	int r = len;
	while (l <= r) {
		int m = l + (r - l) / 2;
		if (str[m] == el)
			return true;
		if (str[m] < el)
			l = m + 1;
		else
			r = m - 1;
	}
	return false;
}
bool esc_sym(int c)
{
	static const char* esc_ = "\"'\\abfnrtv";
	return x_bsearch(esc_, 10, c);
}
bool maybe_kw(int c)
{
	static const char* str = "abdefgilnorstw";
	return x_bsearch(str, 15, c);
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

bool _parse_kw(Scanner* self, String* xtoken, int* xsym, token_type* tt)
{
	int sym = at_str(xtoken, 0);
	const char* predict = NULL;
	int nsym = *xsym;
	bool e = true;
	push_back_str(xtoken, nsym);
	size_t i = 2;

	if (sym == 'e')
	{
		if (nsym == EOF || (!isalnum(nsym) && nsym != '_')) return false;

		if (nsym == 'n') { predict = "end"; *tt = tt_end; goto done; }
		if (nsym != 'l')return false;

		predict = "else"; *tt = tt_else;

		for (; i < 4; i++)
		{
			*xsym = sym = fetch_symbol(self);
			if (sym == EOF || (!isalnum(sym) && sym != '_')) return false;
			push_back_str(xtoken, sym);
			if (sym != predict[i])return false;
		}
		if ((!isalnum(*xsym = sym = fetch_symbol(self)) && sym != '_') || sym == EOF)
			return true;
		i++; push_back_str(xtoken, sym);
		if (sym == 'i') { e = false; predict = "elseif"; *tt = tt_elseif; goto done; }
	}

	switch (sym)
	{
	case 'a':
		predict = "and"; *tt = tt_and; break;
	case 'b':
		predict = "boolean"; *tt = tt_boolean; break;
	case 'd':
		predict = "do"; *tt = tt_do; break;
	case 'g':
		predict = "global"; *tt = tt_global; break;
	case 'l':
		predict = "local"; *tt = tt_local; break;
	case 'o':
		predict = "or"; *tt = tt_or; break;
	case 's':
		predict = "string"; *tt = tt_string; break;
	case 'w':
		predict = "while"; *tt = tt_while; break;

	case 'f':
		e = false;
		switch (nsym)
		{
		case 'a': predict = "false"; *tt = tt_false; break;
		case 'u': predict = "function"; *tt = tt_function; break;
		default:return false;
		}
		break;
	case 'i':
		e = false;
		switch (nsym)
		{
		case 'n': predict = "integer"; *tt = tt_integer; break;
		case 'f': predict = "if"; *tt = tt_if; break;
		default:return false;
		}
		break;
	case 'n':
		e = false;
		switch (nsym)
		{
		case 'i': predict = "nil"; *tt = tt_nil; break;
		case 'o': predict = "not"; *tt = tt_not; break;
		case 'u': predict = "number"; *tt = tt_number; break;
		default:return false;
		}
		break;
	case 't':
		e = false;
		switch (nsym)
		{
		case 'h': predict = "then"; *tt = tt_then; break;
		case 'r': predict = "true"; *tt = tt_true; break;
		default:return false;
		}
		break;
	default:
		if (sym == 'r' && nsym == 'e')
		{
			e = false;
			*xsym = sym = fetch_symbol(self);
			if (sym == EOF || (!isalnum(sym) && sym != '_')) return false;
			i++; push_back_str(xtoken, sym);
			switch (sym)
			{
			case 'q': predict = "require"; *tt = tt_require; break;
			case 't': predict = "return"; *tt = tt_return; break;
			default:return false;
			}
		}
		break;
		return false;
	}
done:
	size_t len = strlen(predict);

	if (e) { if (nsym != predict[i - 1])return false; }

	for (; i < len; i++)
	{
		*xsym = sym = fetch_symbol(self);
		if (sym == EOF || (!isalnum(sym) && sym != '_')) return false;
		push_back_str(xtoken, sym);
		if (sym != predict[i])return false;
	}
	if ((!isalnum(*xsym = sym = fetch_symbol(self)) && sym != '_') || sym == EOF)
		return true;
	push_back_str(xtoken, sym);
	return false;
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
	uint8_t escape_cnt = 0;

	if (self->sym) { sym = self->sym; goto inbound; }

	while (true)
	{
		sym = fetch_symbol(self);
	inbound:
		switch (state)
		{
		case s_begin:
			switch (sym)
			{
			case '+':
				predict = tt_add;
				state = s_plus;
				goto cont_scan;
			case '-':
				predict = tt_subtract;
				state = s_minus;
				goto cont_scan;
			case '*':
				predict = tt_multiply;
				goto simple_tk;
			case '/':
				state = s_div;
				predict = tt_divide;
				goto cont_scan;
			case '%':
				predict = tt_modulo;
				goto simple_tk;
			case '#':
				predict = tt_length;
				goto simple_tk;
			case '(':
				predict = tt_left_parenthese;
				goto simple_tk;
			case ')':
				predict = tt_right_parenthese;
				goto simple_tk;
			case '^':
				predict = tt_power;
				goto simple_tk;
			case '~':
				predict = tt_ne;
				goto simple_tk;
			case ',':
				predict = tt_comma;
				goto simple_tk;
			case ';':
				predict = tt_semicolon;
				goto simple_tk;
			case ':':
				predict = tt_colon;
				goto simple_tk;
			case '>':
				state = s_cmp;
				predict = tt_g;
				goto cont_scan;
			case '<':
				state = s_cmp;
				predict = tt_l;
				goto cont_scan;
			case '=':
				state = s_cmp;
				predict = tt_e;
				goto cont_scan;
			case '.':
				state = s_cat;
				predict = tt_concatenate;
				goto cont_scan;
			case '"':
				state = s_string;
				predict = tt_string_literal;
			cont_scan:
				push_back_str(&xtoken, (char)sym);
				tkstart_col = self->column;
				tkstart_line = self->line;
				/* fall through */
			case ' ':
			case '\n':
				continue;
			case EOF:
				e = e_eof;
				predict = tt_eof;
				token_ctor(tk, predict, NULL);
				tk->line = self->line;
				tk->column = self->column;
				return e;
			default:
				tkstart_col = self->column;
				tkstart_line = self->line;
				if (isalpha(sym) || sym == '_')
				{
					state = maybe_kw(sym) ? s_kw : s_id;
					push_back_str(&xtoken, (char)sym);
					continue;
				}
				if (isdigit(sym))
				{
					state = s_int;
					push_back_str(&xtoken, (char)sym);
					continue;
				}
				sym = 0;
				return e_invalid_token;
			simple_tk:
				token_ctor(tk, predict, NULL);
				tk->line = self->line;
				tk->column = self->column;
				sym = 0;
				return e;
			}
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
		case s_plus:
			if (is_operand(self->prev_state))
				goto make_token_nostr;
			sym = skip_space(self, sym);
			if (isdigit(sym))
			{
				state = s_int;
				push_back_str(&xtoken, (char)sym);
				break;
			}
			predict = tt_u_plus;
			goto make_token_nostr;

		case s_minus:
			if (sym == '-') {
				state = s_commentary;
				break;
			}
			if (is_operand(self->prev_state))
				goto make_token_nostr;
			sym = skip_space(self, sym);
			if (isdigit(sym))
			{
				state = s_int;
				push_back_str(&xtoken, (char)sym);
				break;
			}
			predict = tt_u_minus;
			goto make_token_nostr;
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
			break;
		case s_string:
			if (sym == '"')
			{
				push_back_str(&xtoken, (char)sym);
				sym = 0;
				goto make_token;
			}
			else if (sym == '\\')
			{
				state = s_esc;
			}
			else if (sym == EOF)
			{
				return e_invalid_token;
			}
			push_back_str(&xtoken, (char)sym);
			break;
		case s_cmp:
			if (sym == '=')
			{
				push_back_str(&xtoken, (char)sym);
				predict = (token_type)((uint32_t)predict + 1);
				sym = 0;
			}
			goto make_token;
			break;
		case s_cat:
			if (sym == '.')
			{
				push_back_str(&xtoken, (char)sym);
				sym = 0;
				goto make_token;
			}
			return e_invalid_token;
		case s_div:
			if (sym == '/')
			{
				predict = tt_int_divide;
				push_back_str(&xtoken, (char)sym);
				sym = 0;
			}
			goto make_token;
		case s_ne:
			if (sym == '=')
			{
				push_back_str(&xtoken, (char)sym);
				sym = 0;
				goto make_token;
			}
			return e_invalid_token;
		case s_esc:
			if (esc_sym(sym) && !escape_cnt)
			{
				state = s_string;
				push_back_str(&xtoken, (char)sym);
			}
			else if (isdigit(sym)) {
				push_back_str(&xtoken, (char)sym);
				if (++escape_cnt == 3)
				{
					state = s_string;
					escape_cnt = 0;
					StringView s = substr_b(&xtoken, 3);
					char* end = NULL;
					unsigned long a = strtoul(s.data, &end, 10);
					if (!a || a > 255)return e_invalid_token;
				}
			}
			else return e_invalid_token;
			break;
		case s_commentary:
			if (sym == '[')
			{
				state = s_commentary_ml;
				break;
			}
			sym = skip_comment(self);
			state = s_begin;
			clear_str(&xtoken);
			break;
		case s_commentary_ml:
			if (sym != '[')
				sym = skip_comment(self);
			else
				sym = skip_comment_ml(self);
			state = s_begin;
			clear_str(&xtoken);
			break;
		case s_kw:
			if (sym == EOF || (!isalnum(sym) && sym != '_')) {
				predict = tt_identifier;
				goto make_token;
			}
			if (!_parse_kw(self, &xtoken, &sym, &predict))
			{
				if (sym == EOF || (!isalnum(sym) && sym != '_')) {
					predict = tt_identifier;
					goto make_token;
				}
				state = s_id;
			}
			else goto make_token_nostr;
			break;
		case s_id:
			predict = tt_identifier;
			if (isalnum(sym) || sym == '_')
				push_back_str(&xtoken, sym);
			else
				goto make_token;
		default:
			break;
		check_valid:
			if (!valid)return e_invalid_token;
		make_token:
			token_ctor(tk, predict, &xtoken);
			tk->line = tkstart_line;
			tk->column = tkstart_col;
			return e;
		make_token_nostr:
			token_ctor(tk, predict, NULL);
			tk->line = tkstart_line;
			tk->column = tkstart_col;
			return e;
		}
	}
	return e_eof;
}
