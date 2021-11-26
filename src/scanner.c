#include "scanner.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/*!
 *  States of finite state machine
 *  
 *  @code s_float is moved to 1 << 13
 */
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
	s_clpar,

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


//UNUSED
void _Scanner_run(Scanner* self, Error* e) {
	while (true)
	{
		token* tk = push_back_Vector_token(&(self->tk_stream));

		*e = _get_token(self, tk);
		if (*e != e_ok) {
			e_msg("Scanner error %d", *e);
			break;
		}if (tk->type == tt_eof) {
			break;
		}
	}
}
//UNUSED
void _Scanner_print(Scanner* self) {
	for (size_t i = 0; i < size_Vector_token(&(self->tk_stream)); i++)
	{
		print_tk(at_Vector_token(&(self->tk_stream), i));
	}
}

//returns first symbol
int fetch_symbol(Scanner* self)
{
	int sym = fgetc(self->source);
	if (sym != EOF) {
		self->column++;
		if (sym == '\n') { self->line++; self->column = 0; }
	}
	return sym;
}

/*!
 *  skips everything to end of line or end of file
 *  
 *  @return EOF or \"\\n\"
 */
int skip_comment(Scanner* self)
{
	int sym = 0;
	while (sym != EOF && sym != '\n')
		sym = fetch_symbol(self);
	return sym;
}

/*!
 *  skips everything to ]]
 *  
 *  @return EOF or \"]\"
 */
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

/*!
 *  skips all white spaces except new line
 *  
 *  @return next char
 */
int skip_space(Scanner* self, int sym)
{
	while (sym != EOF && isspace(sym) && sym != '\n')
		sym = fetch_symbol(self);
	return sym;
}

/*!
 *  By masking value of state return bool
 *  
 *  @param state is numeric value of enum
 *	@return TRUE, if state is operand
 */
bool is_operand(uint32_t state)
{
	return state == s_int || (state & 1 << 13) > 0 || state == s_id || state == s_clpar;
}

/*!
 *  Binary search char in string
 *  
 *  @param str string of ordered chars
 *  @param len size of str
 *  @param el char target char
 *  @return TRUE, if char is in str
 */
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

/*!
 *  Detection of posible esc char
 *  @return TRUE, if c is posible esc char
 */
bool esc_sym(int c)
{
	static const char* esc_ = "\"'\\abfnrtv";
	return x_bsearch(esc_, 10, c);
}

/*!
 *  Detection of posible kw char
 *  @return TRUE, if c is posible kw char
 */
bool maybe_kw(int c)
{
	static const char* str = "abdefgilnorstuw";
	return x_bsearch(str, 16, c);
}

//Storage of pointers to new/old states and symbols
typedef struct
{
	uint32_t* state_to;
	State* state_from;
	int* sym_to;
	int* sym_from;
}sym_guard;

//copy states and symbols from new to old
void sym_guard_dtor(sym_guard* self)
{
	*self->sym_to = *self->sym_from;
	*self->state_to = *self->state_from;
}

/*!
 *  Sub-finite-state machine for keywords
 *  @param self pointer to scanner
 *  @param xtoken pointer to token
 *  @param xsym pointer to next symbol
 *  @param tt pointer to token type
 *  @return TRUE, if token is keyword
 */
bool _parse_kw(Scanner* self, String* xtoken, int* xsym, token_type* tt)
{
	//first token
	int sym = at_str(xtoken, 0);
	const char* predict = NULL;
	//next token
	int nsym = *xsym;
	push_back_str(xtoken, nsym);
	//starting index of check loop
	size_t i = 2;
	//sets prediction by first two chars or more
	switch (sym)
	{
	case 'a':
		predict = "and"; *tt = tt_and; break;

	case 'b':
		predict = "boolean"; *tt = tt_boolean; break;

	case 'd':
		predict = "do"; *tt = tt_do; break;

	case 'e':
		if (nsym == EOF || (!isalnum(nsym) && nsym != '_')) return false;

		if (nsym == 'n') { predict = "end"; *tt = tt_end; goto done; }

		if (nsym != 'l') return false;

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
		if (sym == 'i') { predict = "elseif"; *tt = tt_elseif; goto done; }
		break;

	case 'f':
		switch (nsym)
		{
			case 'a': predict = "false"; *tt = tt_false; break;
			case 'u': predict = "function"; *tt = tt_function; break;
			case 'o': predict = "for"; *tt = tt_for; break;
			default:return false;
		}
		break;

	case 'g':
		predict = "global"; *tt = tt_global; break;

	case 'i':
		switch (nsym)
		{
			case 'n': predict = "integer"; *tt = tt_integer; break;
			case 'f': predict = "if"; *tt = tt_if; break;
			default:return false;
		}
		break;

	case 'l':
		predict = "local"; *tt = tt_local; break;

	case 'n':
		switch (nsym)
		{
			case 'i': predict = "nil"; *tt = tt_nil; break;
			case 'o': predict = "not"; *tt = tt_not; break;
			case 'u': predict = "number"; *tt = tt_number; break;
			default: return false;
		}
		break;

	case 'o':
		predict = "or"; *tt = tt_or; break;

	case 's':
		predict = "string"; *tt = tt_string; break;

	case 'r':
		if (nsym == 'e')
		{
			*xsym = sym = fetch_symbol(self);
			if (sym == EOF || (!isalnum(sym) && sym != '_')) return false;
			i++; push_back_str(xtoken, sym);
			switch (sym)
			{
				case 'q': predict = "require"; *tt = tt_require; break;
				case 't': predict = "return"; *tt = tt_return; break;
				case 'p': predict = "repeat"; *tt = tt_repeat; break;
				default: return false;
			}
			break;
		}
		break;

	case 't':
		switch (nsym)
		{
			case 'h': predict = "then"; *tt = tt_then; break;
			case 'r': predict = "true"; *tt = tt_true; break;
			default:return false;
		}
		break;

	case 'u':
		predict = "until"; *tt = tt_until; break;
	case 'w':
		predict = "while"; *tt = tt_while; break;
	default:
		//no match was found
		return false;
	}
	//end of prediction switch
done:
	if (predict == NULL) {
		e_msg("Internal scanner error");
		return false;
	}

	//checks if prediction is same as string
	for (; i < strlen(predict); i++)
	{
		*xsym = sym = fetch_symbol(self);
		if (sym == EOF || (!isalnum(sym) && sym != '_')) return false;
		push_back_str(xtoken, sym);
		if (sym != predict[i]) return false;
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


	Error e = e_ok;
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
				state = s_clpar;
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
				predict = tt_assign;
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
			case ' ' :
			case '\t':
			case '\n':
			case '\v':
				continue;
			case EOF:
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
		//start of states
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
			else if (sym == EOF || sym == '\n')
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
				state = s_id;
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
	return e_invalid_token;
}


Error get_token(Scanner* self, token* tk)
{
	if (empty_Vector_token(&self->tk_stream))
		return _get_token(self, tk);
	token* xtk = back_Vector_token(&self->tk_stream); //token is moved
	*tk = *xtk;
	xtk->var = v_none;
	DEBUG_ZERO(xtk);
	pop_back_Vector_token(&self->tk_stream);
	return e_ok;
}
void unget_token(Scanner* self, token* tk)
{
	*push_back_Vector_token(&self->tk_stream) = *tk;
	tk->var = v_none;
	DEBUG_ZERO(tk);
}
