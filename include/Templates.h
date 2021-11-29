#pragma once

#include "ast.h"
#define IsClass
#define VECTOR_T token
#include "VLib/Vector_T.h"

#define IsClass
#define VECTOR_T Node
#include "VLib/Vector_T.h"

#define VECTOR_T ptrdiff_t
#include "VLib/Vector_T.h"

#include "syntax.h"
#define IsClass
#define VECTOR_T ppIASTElement
#include "VLib/Vector_T.h"

#define VECTOR_T token_type
#include "VLib/Vector_T.h"

#include "function.h"

#define HASH_T FunctionDecl
#include "symtable.h"

typedef struct
{
	token_type type;
	bool has_value;
}Variable;

#define HASH_T Variable
#include "symtable.h"

#define VECTOR_T HashMap(Variable)
#define IsClass
#include "VLib/Vector_T.h"

