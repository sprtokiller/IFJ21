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

#define VECTOR_T token_type
#include "VLib/Vector_T.h"

#include "syntax.h"
#define IsClass
#define VECTOR_T ppIASTElement
#include "VLib/Vector_T.h"

#include "symtable.h"
#define IsClass
#define VECTOR_T Sym_table
#include "VLib/Vector_T.h"
