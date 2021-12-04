#include "builtin.h"
#include <string.h>

const char* xwrite =
"# write(...)\n"
"# argument count is first\n"
"LABEL $$_builtin_write\n"
	"CREATEFRAME\n"
	"PUSHFRAME\n"
	"DEFVAR LF@ecx #Argument counter\n"
	"DEFVAR LF@eax #bool\n"
	"POPS LF@ecx\n"
	"DEFVAR LF@curr #Current symbol\n"
"# LOOP START\n"
	"JUMP $write_cond\n"
"LABEL $write_loop\n"
"# {\n"
	"POPS LF@curr\n"
	"WRITE LF@curr\n"
"# }\n"
"LABEL $write_cond\n"
	"GT LF@eax LF@ecx int@0\n"
	"SUB LF@ecx LF@ecx int@1\n"
	"JUMPIFEQ $write_loop LF@eax bool@true\n"
	"POPFRAME\n"
	"RETURN\n";


const char* xreads =
	"# string reads()\n"
"LABEL $$_builtin_reads\n"
	"CREATEFRAME\n"
	"PUSHFRAME\n"
	"DEFVAR LF@in\n"
	"READ LF@in string\n"
	"PUSHS LF@in\n"
	"POPFRAME\n"
	"RETURN\n";

const char* xreadi =
	"# int readi()\n"
"LABEL $$_builtin_readi\n"
	"CREATEFRAME\n"
	"PUSHFRAME\n"
	"DEFVAR LF@in\n"
	"READ LF@in int\n"
	"PUSHS LF@in\n"
	"POPFRAME\n"
	"RETURN\n";

const char* xreadn =
	"# float readn()\n"
"LABEL $$_builtin_readn\n"
	"CREATEFRAME\n"
	"PUSHFRAME\n"
	"DEFVAR LF@in\n"
	"READ LF@in float\n"
	"PUSHS LF@in\n"
	"POPFRAME\n"
	"RETURN\n";


const char* xtointeger =
	"# int tointeger(float)\n"
"LABEL $$_builtin_tointeger\n"
	"CREATEFRAME\n"
	"PUSHFRAME\n"
	"DEFVAR LF@in\n"
	"POPS LF@in\n"
	"JUMPIFNEQ $toint_cont LF@in nil@nil\n"
	"PUSHS LF@in\n"
	"POPFRAME\n"
	"RETURN\n"
"LABEL $toint_cont\n"
	"PUSHS LF@in\n"
	"FLOAT2INTS\n"
	"POPFRAME\n"
	"RETURN\n";

const char* xsubstr = 
	"# substr(string, i, j)\n"
"LABEL $$_builtin_substr\n"
	"CREATEFRAME\n"
	"PUSHFRAME\n"

	"# Save input\n"

	"DEFVAR LF@str\n"
	"POPS LF@str\n"

	"DEFVAR LF@out\n"
	"MOVE LF@out string@\n"
	"DEFVAR LF@edx\n"

	"DEFVAR LF@eax #bool\n"
	"DEFVAR LF@ecx\n"
	"STRLEN LF@ecx LF@str\n"

	"DEFVAR LF@esi\n"
	"POPS LF@esi\n"
	"DEFVAR LF@edi\n"
	"POPS LF@edi\n"

	"GT LF@eax LF@esi LF@ecx\n"
	"JUMPIFEQ $substr_err LF@eax bool@true\n"
	"LT LF@eax LF@esi int@1\n"
	"JUMPIFEQ $substr_err LF@eax bool@true\n"

	"GT LF@eax LF@edi LF@ecx\n"
	"JUMPIFEQ $substr_err LF@eax bool@true\n"
	"LT LF@eax LF@edi int@1\n"
	"JUMPIFEQ $substr_err LF@eax bool@true\n"
	"LT LF@eax LF@edi LF@esi\n"
	"JUMPIFEQ $substr_err LF@eax bool@true\n"

"LABEL $substr_cycle\n"
	"GETCHAR LF@edx LF@str LF@esi\n"
	"CONCAT LF@out LF@out LF@edx\n"
	"ADD LF@esi LF@esi int@1\n"
	"GT LF@eax LF@esi LF@edi\n"
	"JUMPIFEQ $substr_cycle LF@eax LF@false\n"

	"PUSHS LF@out\n"
	"POPFRAME\n"
	"RETURN\n"

"LABEL $substr_err\n"
	"PUSHS string@\n" //goes out
	"POPFRAME\n"
	"RETURN\n";


const char* xord =
	"# int ord()\n"
"LABEL $$_builtin_ord\n"
	"CREATEFRAME\n"
	"PUSHFRAME\n"

	"DEFVAR LF@str\n"
	"POPS LF@str\n"

	"DEFVAR LF@eax #bool\n"
	"DEFVAR LF@ecx\n"
	"STRLEN LF@ecx LF@str\n"

	"DEFVAR LF@esi\n"
	"POPS LF@esi\n"

	"GT LF@eax LF@esi LF@ecx\n"
	"JUMPIFNEQ $ord_cont LF@eax LF@false\n"
	"PUSHS nil@nil\n"
	"POPFRAME\n"
	"RETURN\n"

"LABEL $ord_cont\n"
	"STRI2INT LF@ecx LF@str LF@esi\n"
	"PUSHS LF@ecx\n"
	"POPFRAME\n"
	"RETURN\n";


const char* xchr =
	"# int chr()\n"
"LABEL $$_builtin_chr\n"
	"CREATEFRAME\n"
	"PUSHFRAME\n"

	"DEFVAR LF@in\n"
	"POPS LF@in\n"

	"DEFVAR LF@eax #bool\n"

	"LT LF@eax LF@in int@0\n"
	"JUMPIFEQ $chr_err LF@eax LF@true\n"

	"GT LF@eax LF@in int@255\n"
	"JUMPIFEQ $chr_err LF@eax LF@true\n"

	"PUSHS LF@in\n"
	"INT2CHARS\n"
	"POPFRAME\n"
	"RETURN\n"

"LABEL $chr_err\n"
	"PUSHS nil@nil\n"
	"POPFRAME\n"
	"RETURN\n";

// a/(a-a*b+b)
const char* xpow = 
"# float pow(float, int)\n"
"LABEL $$_builtin_pow\n"
	"CREATEFRAME\n"
	"PUSHFRAME\n"
	
	"DEFVAR LF@in\n"
	"POPS LF@in\n"
	
	"DEFVAR LF@ecx\n"
	"POPS LF@ecx\n"
	
	"DEFVAR LF@ebx # res\n"
	"MOVE LF@ebx float@0x8p-3\n" //1.0

	"DEFVAR LF@eax #bool\n"
	"JUMPIFEQ $_pow_one LF@ecx int@0\n"
	"LT LF@eax LF@ecx LF@0\n"
	"JUMPIFNEQ $_pow_pos LF@eax bool@true\n"

	"DIV LF@in float@0x8p-3 LF@in\n" //flip the bird
	"SUB LF@ecx int@0 LF@ecx\n"

"LABEL $_pow_pos\n"
	"MUL LF@ebx LF@ebx LF@in\n"
	"SUB LF@ecx LF@ecx int@1\n"
	"JUMPIFNEQ $_pow_pos LF@ecx int@0\n"
	
"LABEL $_pow_one\n"
	"PUSHS LF@ebx\n"
	"POPFRAME\n"
	"RETURN\n";
	


const token_type ttchr_i = tt_integer;
const token_type ttchr_o = tt_string;
const token_type ttreadn_o = tt_number;

const token_type ttsubstr_i[] = { tt_string, tt_integer, tt_integer };

const FunctionDecl fdchr = {
	.types = {&ttchr_i, &ttchr_i + 1},
	.ret = {&ttchr_o, &ttchr_o + 1},
	.proto = false,
	.called = false
};
const FunctionDecl fdord = {
	.types = {&ttchr_o, &ttchr_o + 1},
	.ret = {&ttchr_i, &ttchr_i + 1},
	.proto = false,
	.called = false
};
const FunctionDecl fdreadi = {
	.types = {NULL, NULL},
	.ret = {&ttchr_i, &ttchr_i + 1},
	.proto = false,
	.called = false
};
const FunctionDecl fdreads = {
	.types = {NULL, NULL},
	.ret = {&ttchr_o, &ttchr_o + 1},
	.proto = false,
	.called = false
};
const FunctionDecl fdreadn = {
	.types = {NULL, NULL},
	.ret = {&ttreadn_o, &ttreadn_o + 1},
	.proto = false,
	.called = false
};
const FunctionDecl fdsubstr = {
	.types = {&ttsubstr_i[0], &ttsubstr_i[3]},
	.ret = {&ttchr_o, &ttchr_o + 1},
	.proto = false,
	.called = false
};
const FunctionDecl fdtointeger = {
	.types = {&ttreadn_o, &ttreadn_o + 1},
	.ret = {&ttchr_i, &ttchr_i + 1},
	.proto = false,
	.called = false
};
const FunctionDecl fdwrite = {
	.types = {NULL, NULL},
	.ret = {NULL, NULL},
	.proto = false,
	.called = false
};


extern bool x_bsearch(const char* str, size_t len, int el);

bool IsBuiltin(const char* func_id)
{
	static const char* str = "corstw";
	const char c = func_id[0];
	if(!x_bsearch(str, 6, c))return false;
	switch (c)
	{
	case 'c':
		return !strcmp(func_id, "chr");
	case 'o':
		return !strcmp(func_id, "ord");
	case 'r':
		return !strcmp(func_id, "reads") || !strcmp(func_id, "readi") || !strcmp(func_id, "readn");
	case 's':
		return !strcmp(func_id, "substr");
	case 't':
		return !strcmp(func_id, "tointeger");
	case 'w':
		return !strcmp(func_id, "write");
	default:
		return false;
	}
}

bool IsWrite(const char* func_id)
{
	return !strcmp(func_id, "write");
}
