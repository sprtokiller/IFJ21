#define PARSER_IMPL
#include "../include/parser.h"

void Constructor(selfptr)
{
	Scanner_ctor(&self->scan, stdin);
}
void Destructor(selfptr)
{
	Scanner_dtor(&self->scan);
}






void Start()
{

}
