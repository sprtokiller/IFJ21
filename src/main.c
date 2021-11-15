#include <stdlib.h>
#include <stdio.h>

#include "symtable.h"
#include "common.h"
#include "token.h"
#include "scanner.h"
#include "syntax.h"

int main(int argc, char* argv[])
{
	SetupTerminal();

	bool verbose = false;
	FILE* stream = stdin;

	size_t argIndex;
	for (argIndex = 1; argIndex < argc && argv[argIndex][0] == '-'; argIndex++) {
		switch (argv[argIndex][1]) {
		case 'v': verbose = true; break;
		case 'h': usage(argv[0]); exit(1); break;
		case 's':
			Scanner sc;
			Scanner_ctor(&sc, stream);
			Error e;
			Scanner_run(&sc, &e);
			Scanner_print(&sc);
			Scanner_dtor(&sc);
			return 0;
		case 't':
			Sym_table st = { 0 };
			Sym_table_fill(&st, stream);
			Sym_table_print(&st);
			Sym_table_dtor(&st);
			return 0;
		default: usage(argv[0]); exit(64); break;
		}
	}

	stream = fopen(SOURCE_DIR"/examples/prg_2.tl",'r');

	Sym_table st = { 0 };
	Sym_table_fill(&st, stream);
	rule Rules[50][50] = {};
	Syntax_init(&Rules);
	Syntax_run(&Rules,&st);
	Sym_table_dtor(&st);

	return 0;
}
