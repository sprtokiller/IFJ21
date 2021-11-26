#pragma once

#define ENUM_ERROR_TYPES \
	X(ok) X(invalid_token) X(invalid_syntax) X(eof) 

typedef enum Error {
#define X(a) e_##a,
	ENUM_ERROR_TYPES
#undef X
}Error;

const char* error_type_name(Error type);