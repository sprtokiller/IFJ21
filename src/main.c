#include "common.h"
#include "../include/parser.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
	SetupTerminal();

	FILE* stream = fopen(SOURCE_DIR "/examples/testnum.tl","r");

	Parser parser;
	Parser_ctor(&parser, stream);
	Start(&parser);

	Parser_dtor(&parser);

	return 0;
}
