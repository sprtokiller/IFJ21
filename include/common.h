#pragma once
#include "error.h"

#define SWITCH_TEXT(tt, str) case tt: return str
#define ERR_CHECK(call) if((e = call)>0) return e;

///Color change on tests for better readability
#ifdef RAW_OUTPUT
#define RED     ""
#define GREEN   ""
#define YELLOW  ""
#define RESET   ""
#else
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define RESET   "\x1b[0m"
#endif // RAW_OUTPUT

void usage(char* prgName);

/// @briefprints debug in stderr
/// @param fmt format string
/// @param variables
void d_msg(const char* fmt, ...);

/// @briefprints warning in stderr
/// @param fmt format string
/// @param variables
void w_msg(const char* fmt, ...);

/// @brief prints error in stderr
/// @param fmt format string
/// @param variables
void e_msg(const char* fmt, ...);


/// @brief prints error in stderr
/// and shuts down program
/// @param fmt format string
/// @param variables
void e_exit(const char* fmt, ...);

/// @brief Sets up terminal on Windows
void SetupTerminal(void);
