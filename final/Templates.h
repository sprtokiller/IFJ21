#pragma once

#include "ast.h"
#define IsClass
#define VECTOR_T token
#include "Vector_T.h"

#define IsClass
#define VECTOR_T Node
#include "Vector_T.h"

#define VECTOR_T ptrdiff_t
#include "Vector_T.h"

#include "syntax.h"
#define IsClass
#define VECTOR_T ppIASTElement
#include "Vector_T.h"

#define VECTOR_T token_type
#include "Vector_T.h"

#include "function.h"

#define HASH_T FunctionDecl
#define IsClass
#include "symtable.h"

typedef struct
{
	const char* asm_name;
	token_type type;
	bool has_value;
	bool global;
}Variable;

#define HASH_T Variable
#include "symtable.h"

#define VECTOR_T HashMap(Variable)
#define IsClass
#include "Vector_T.h"

#define VECTOR_T String
#define IsClass
#include "Vector_T.h"

