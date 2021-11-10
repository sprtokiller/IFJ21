#include <stdlib.h>
#include "symtable.h"
#include "common.h"
#include "token.h"
#include "scanner.h"

int main(int argc, char* argv[])
{
	SetupTerminal();

	bool verbose = false;
	size_t argIndex;
	for (argIndex = 1; argIndex < argc && argv[argIndex][0] == '-'; argIndex++) {
		switch (argv[argIndex][1]) {
		case 'v': verbose = true; break;
		case 'h': usage(argv[0]); exit(1); break;
		default: usage(argv[0]); exit(64); break;
		}
	}

	FILE* stream = stdin;

	Sym_table st = {0};
	Sym_table_fill(&st, stream);
	Sym_table_print(&st);
	Sym_table_dtor(&st);
	return 0;
}
