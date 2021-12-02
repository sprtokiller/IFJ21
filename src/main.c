#include "common.h"
#include "../include/parser.h"
#include "semantic.h"
#include "codegen.h"
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
	if (e) { e_msg(error_type_name(e)); return e; }

	UNIQUE(CodeGen) cg = {0};
	CodeGen_ctor(&cg, &semantic.funcs);

	(*parser.program)->generate(parser.program, &cg);


	return e;
}
