#include "error.h"

const char* error_types_str[] =
{
#define X(a) #a,
	ENUM_ERROR_TYPES
#undef X
};

const char* error_type_name(Error type) {
	return error_types_str[type];
}