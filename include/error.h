#pragma once

#define ENUM_ERROR_TYPES \
	X(ok) X(invalid_token) X(invalid_syntax) X(redefinition) X(type_ass) X(count) X(type) X(other) X(RTnil) X(RTzero) 

typedef enum Error {
#define X(a) e_##a,
	ENUM_ERROR_TYPES
#undef X
}Error;

const char* error_type_name(Error type);