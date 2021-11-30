#include "error.h"

const char* error_types_str[] = {
	"no error",
	"lexical error or invalid token",
	"syntax error",
	"variable redefinition",
	"assignment types mismatch",
	"wrong count of return or input types",
	"expresion type mismatch",
	"other semantic",
	"Runtime Prediction: nil value operation",
	"Runtime Prediction: zero division",
	"compiler internal error"
};

const char* error_type_name(Error type) {
	return error_types_str[(unsigned)type];
}