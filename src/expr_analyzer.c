#define EXPRA_IMPL
#include "expr_analyzer.h"
#include "scanner.h"

const ExprAction precedence_table[][13] =
{
	{S,R,R,R,R,S,R,R,R,R,R,S,R},
	{X,X,R,R,X,S,R,R,R,R,R,S,R},
	{S,S,R,R,X,S,R,R,R,R,R,S,R},
	{S,S,S,R,X,S,R,R,R,R,R,S,R},
	{X,X,X,X,S,S,R,R,X,X,R,S,R},
	{S,S,S,S,S,S,E,S,S,S,X,S,S},
	{R,X,R,R,S,X,R,R,R,R,R,X,X},
	{S,S,S,S,S,S,R,R,R,R,R,S,R},
	{S,S,S,S,S,S,R,S,R,R,R,S,R},
	{S,S,S,S,S,S,R,S,S,R,R,S,R},
	{S,S,S,S,S,S,X,S,S,S,S,S,X},
	{R,X,R,R,R,S,R,R,R,R,R,X,R},
	{S,S,S,S,S,S,R,S,S,S,X,S,S}
};

void Constructor(ExpressionAnalyzer* sel, Scanner* scanner)
{

}

void Destructor(ExpressionAnalyzer* sel)
{

}
