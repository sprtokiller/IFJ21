#include "common.h"
#include "../include/parser.h"
#include "semantic.h"
#include <stdio.h>
	
int main(int argc, char* argv[])
{
	SetupTerminal();
	Error e = e_ok;

	FILE* stream = fopen(SOURCE_DIR "/examples/testnum.tl","r");

	UNIQUE(SemanticAnalyzer) semantic = {0};
	SemanticAnalyzer_ctor(&semantic);

	UNIQUE(Parser) parser;
	Parser_ctor(&parser, stream);
	ERR_CHECK(Start(&parser));
	e = (*parser.program)->analyze(parser.program, &semantic);
	printf(error_type_name(e));

	return e;
}
