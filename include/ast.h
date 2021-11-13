#pragma once
#include "../include/token.h"
#include <VLib/RuntimeClass.h>

#pragma push_macro("c_class")
#undef c_class
#define c_class Node
typedef struct Node Node;

void Constructor(selfptr);
void Destructor(selfptr);


struct Node {
	bool valid;
	token core;
	Node* left, *right;           /**< Left and right subtree */
};

token* Node_emplace(selfptr);


#ifndef NODE_IMPL
#pragma pop_macro("c_class")
#endif