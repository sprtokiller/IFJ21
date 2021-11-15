#include "common.h"
#include "expr_analyzer.h"
#include "scanner.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
	SetupTerminal();

	FILE* stream = fopen(SOURCE_DIR "/examples/testnum.tl","r");

	Scanner scan;
	Scanner_ctor(&scan, stream);

	ExpressionAnalyzer ea;
	ExpressionAnalyzer_ctor(&ea);

	Error e = e_ok;
	if (e = Execute(&ea, &scan) != e_ok) { printf("error"); return 1; };
	print_tree(front_Vector_Node(&ea.ast));

	return 0;
}
