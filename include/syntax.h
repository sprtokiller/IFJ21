#pragma once
#include "token.h"
#include "error.h"

typedef enum
{
	s_await,
	s_accept,
	s_refused,
	s_await_e,
	s_fcall,
	s_accept_fcall
}RetState;

struct SemanticAnalyzer;
struct CodeGen;

typedef struct IASTElement
{
	RetState(*append)(struct IASTElement** self, token* tk);
	void(*print)(struct IASTElement** self);
	void(*dtor)(struct IASTElement** self);
	Error(*analyze)(struct IASTElement** self, struct SemanticAnalyzer* analyzer);
	void (*generate)(const struct IASTElement** self, struct CodeGen* codegen);
}IASTElement,**ppIASTElement;

void ppIASTElement_dtor(ppIASTElement* self);

IASTElement** MakeStatement(token_type type);
IASTElement** MakeProgram();

