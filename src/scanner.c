#include <stdio.h>
#include <stdlib.h>
#include <scanner.h>
#include <ctype.h>
#include "common.h"
#include "token.h"

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
	uint max = 4;
	w = (char*)malloc(max);
	if (!w) {
		e_msg("Alocation failed.\n");
		return NULL;
	}

	//curently used chars in word
	uint count = 0;

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

//Returns next token
token_s* get_t(FILE* f) {
	//scaned word
	char* w = get_word(f);
	// TODO: implement line, colum numbering
	token_s* t = init_t(w, 0, 0);
	return t;
}
