#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <scanner.h>
#include <ctype.h>
#include "common.h"
#include "token.h"
#include "llc.h"

//gets word that can be used in token from stream
char* get_word(FILE* f) {
	char c;
	char* w;
	//skip all white spaces
	do
	{
		c = fgetc(f);
	} while (isspace(c));

	//handeling empty file or EOF at end of file
	if (c == EOF || c == '\x1a') {
		w = (char*)malloc(2);
		if (!w) {
			e_msg("Alocation failed.\n");
			return NULL;
		}
		w[0] = EOF;
		w[1] = '\0';
		return w;
	}

	//max chars per word
	uint32_t max = 4;
	w = (char*)malloc(max);
	if (!w) {
		e_msg("Alocation failed.\n");
		return NULL;
	}

	//curently used chars in word
	uint32_t count = 0;

	//scan words
	// TODO: missing another seperators and logic
	while (!isspace(c)) {
		if (c == EOF || c == '\x1a') {
			w_msg("Unexpected end of file.");
			ungetc(c, f);
			break;
		}
		w[count++] = c;
		//check if chars can fit, if not expands space
		if (count == max - 1) {
			max *= 2;
			void* tmp = realloc(w, max);
			if (!tmp) {
				e_msg("Realocation failed.\n");
				free(w);
				return NULL;
			}
			w = (char*)tmp;
		}
		c = fgetc(f);
	}

	w[count] = '\0';
	return w;
}

char get_next_nonwhite(FILE* f, coord_s* coord) {
	char c = fgetc(f);
	coord->column++;
	while (isspace(c) && c != EOF) {
		c = fgetc(f);
		if (c == '\n')
		{
			coord->column = 0;
			coord->line++;
		}
		else
		{
			coord->column++;
		}
	}
	return c;
}

token_s* tilda_state(FILE* f, LLC_s* llc, coord_s* coord) {
	token_s* token;
	char c = fgetc(f);
	if (c == '\n')
	{
		coord->column = 0;
		coord->line++;
	}
	else
	{
		coord->column++;
	}

	switch (c)
	{
	case '=':
		token = init_t(NULL, coord->line, coord->column, tt_ne, tg_operator);
		break;
	default:
		token = init_t(NULL, coord->line, coord->column, tt_err, tg_err);
		break;
	}
	return token;
}

token_s* enter_the_state_machine(FILE* f, LLC_s* llc, char c, coord_s* coord) {
	token_s* token;
	switch (c)
	{
	case '+':
		token = init_t(NULL, coord->line, coord->column, tt_add, tg_operator);
		break;
	case '*':
		token = init_t(NULL, coord->line, coord->column, tt_multiply, tg_operator);
		break;
	case '/':
		token = init_t(NULL, coord->line, coord->column, tt_divide, tg_operator);
		break;
	case '%':
		token = init_t(NULL, coord->line, coord->column, tt_modulo, tg_operator);
		break;
	case ':':
		token = init_t(NULL, coord->line, coord->column, tt_semicolumn, tg_special);
		break;
	case '^':
		token = init_t(NULL, coord->line, coord->column, tt_power, tg_operator);
		break;
	case '(':
		token = init_t(NULL, coord->line, coord->column, tt_left_parenthese, tg_special);
		break;
	case ')':
		token = init_t(NULL, coord->line, coord->column, tt_right_parenthese, tg_special);
		break;
	case '#':
		token = init_t(NULL, coord->line, coord->column, tt_length, tg_operator);
		break;

	case '~':
		token = tilda_state(f,llc, coord);
		break;
	
	default:
		//TODO
		token = init_t(NULL, coord->line, coord->column, tt_err, tg_err);
		break;
	}
	return token;
}

//Returns next token
token_s* get_t(FILE* f, LLC_s* llc, coord_s* coord) {

	//skip epsilon transition of whitespace
	char c = get_next_nonwhite(f, coord);
	LLC_push(llc, c);

	token_s* token = enter_the_state_machine(f, llc, c, coord);
	
	return token;
}
