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

#define HASH_T token_type
#include "symtable.h"

#define VECTOR_T HashMap(token_type)
#define IsClass
#include "VLib/Vector_T.h"

