#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "common.h"
#include "scanner.h"
#include "token.h"
#include "llc.h"
#ifdef _WIN32
	#include "windows.h"
#endif // _WIN32

int main(int argc, char* argv[])
{
#ifdef _WIN32
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
#endif // _WIN32

	bool verbose = false;
	size_t argIndex;
	for (argIndex = 1; argIndex < argc && argv[argIndex][0] == '-'; argIndex++) {
		switch (argv[argIndex][1]) {
		case 'v': verbose = true; break;
		case 'h': usage(argv[0]); exit(EXIT_FAILURE); break;
		default: usage(argv[0]); exit(EXIT_BAD_USAGE); break;
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

	FILE* fp = fopen("../../../../examples/testnum.tl", "r");

	Scanner sc;
	Scanner_ctor(&sc, fp);
	token tk;
	Error e = Ok;

	while (true)
	{
		e = _get_token(&sc, &tk);
		if (e == e_eof)break;
		print_tk(&tk);
		token_dtor(&tk);
	}
	Scanner_dtor(&sc);


	return EXIT_SUCCESS;
}
