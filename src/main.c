#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "common.h"
#include "scanner.h"


int main(int argc, char *argv[])
{
	bool verbose = false;
	size_t argIndex;
	for (argIndex = 1; argIndex < argc && argv[argIndex][0] == '-'; argIndex++) {
		switch (argv[argIndex][1]) {
		case 'v': verbose = true; break;
		case 'h': usage(argv[0]); exit(EXIT_FAILURE); break;
		default: usage(argv[0]); exit(EXIT_BAD_USAGE); break;
		}
	}

	char c;
	while ((c = getc(stdin)) != EOF)
	{
		putc(c, stdout);
	}
	
	return EXIT_SUCCESS;
}