#include "scanner.h"
#include "common.h"

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
	
	FILE* fp = fopen(SOURCE_DIR"/examples/scan_1.tl", "r");

	Scanner sc;
	Scanner_ctor(&sc, fp);
	Error e = Ok;
	Scanner_print(&sc, &e);
	Scanner_dtor(&sc);


	return 0;
}
