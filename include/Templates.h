#pragma once

#include "ast.h"
#define IsClass
#define VECTOR_T token
#include "VLib/Vector_T.h"

#define IsClass
#define VECTOR_T Node
#include "VLib/Vector_T.h"

typedef Node* PNode;
#define VECTOR_T PNode
#include "VLib/Vector_T.h"
