#include "syntax.h"

rule test(){
	printf("%s", "ff");
}

void Syntax_init(rules Rules){
	(*Rules)[0][0] = &test;
}

bool Syntax_run(rules Rules, Sym_table st) {
	return true;
}