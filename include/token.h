#ifndef token_h
#define token_h
#include "common.h"

//dont use this!
//just for editors autocomplete
enum token_type { id, dt, kw, lit, br, bl, op, lab, sem, com, as, eof };

//enum type of token
//identificator, data type, key word, literal(int,flt,str), bracket,
//operator, lable, semicolum, comma, assignment, end of file
typedef enum token_type token_t;

//dont use this!
//just for editors autocomplete
struct token_struct {
	uint line;
	uint colum;
	char* warning;

	token_t type;
	char* val;
};

//stuct of token
typedef struct token_struct token_s;

//converts enum to string
const char* type_name(token_t type);

//allocates token and inicializes it
token_s* init_t(char* val, uint line, uint col);

//prints token information
void print_t(token_s* t);
#endif
