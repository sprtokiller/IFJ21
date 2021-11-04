#include "symtable.h"
#include "common.h"
#include "token.h"
#include <stdlib.h>

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



	//LLC_s* llc = LLC_init(64);
	//coord_s coords = { .column = 1, .line = 1 };

	//if (!llc) {
	//	e_msg("Alocation failed.\n");
	//	return 1;
	//}

	////prints all tokens recived stops at EOF
	//token_s* t = NULL;
	//do
	//{
	//	if (t) free(t);
	//	t = get_t(stdin, llc, &coords);
	//	if (!t) break;
	//	print_t(t);
	//} while ((t->group != tg_eof) && (t->group != tg_err));


	//LLC_destroy(llc);

	Sym_table t = {0};

	const char* in1 = "id1";

	Sym_table_insert(&t, in1, tt_l, 0);

	const char* in2 = "id2";

	Sym_table_insert(&t, in2, tt_add, 0);

	hashtable_item* item;

	item = Sym_table_find(&t, in1);
	item = Sym_table_find(&t, in2);

	Sym_table_dtor(&t);

	return 0;
}
