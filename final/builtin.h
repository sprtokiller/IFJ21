#pragma once
#include "function.h"

#define LIST_BUILTINS()\
X(chr)\
X(ord)\
X(reads)\
X(readi)\
X(readn)\
X(substr)\
X(tointeger)\
X(write)


typedef enum BuiltIns
{
#define X(a) f_##a,
	LIST_BUILTINS()
#undef X
}BuiltIns;

#define X(a) extern const char* x##a;
	LIST_BUILTINS()
#undef X

#define X(a) extern const FunctionDecl fd##a;
	LIST_BUILTINS()
#undef X

extern const char* xpow;

bool IsBuiltin(const char* func_id);
bool IsWrite(const char* func_id);