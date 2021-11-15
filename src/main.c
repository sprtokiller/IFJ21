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

	Execute(&ea, &scan);

	return 0;
}
