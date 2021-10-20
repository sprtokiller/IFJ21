#pragma once
#include <stdint.h>
#define EXIT_BAD_USAGE 64

typedef enum Error
{
	Ok,
	e_eof,
	e_invalid_token
}Error;

void usage(char* prgName);

//prints debug in stderr
void d_msg(const char* fmt, ...);

//prints warning in stderr
void w_msg(const char* fmt, ...);

//prints error in stderr
void e_msg(const char* fmt, ...);

//prints error in stderr
//and shuts down program
void e_exit(const char* fmt, ...);
