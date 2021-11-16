#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "common.h"

// TODO: keep technique DRY alive
// i didnt know how to pass va_list to function

void usage(char* prgName)
{
	printf("Usage: %s [-h] < input_tl_prg > output_ifj21_prg\n", prgName);
}

void d_msg(const char* fmt, ...) {
#ifdef RAW_OUTPUT
	fprintf(stderr, "D:");
#else
	fprintf(stderr, GREEN "Debug: " RESET);
#endif // RAW_OUTPUT
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fprintf(stderr, "\n");
}

void w_msg(const char* fmt, ...) {
#ifdef RAW_OUTPUT
	fprintf(stderr, "W:");
#else
	fprintf(stderr, YELLOW "Warn : " RESET);
#endif // RAW_OUTPUT
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fprintf(stderr, "\n");
}

void e_msg(const char* fmt, ...) {
#ifdef RAW_OUTPUT
	fprintf(stderr, "E:");
#else
	fprintf(stderr, RED "Error: " RESET);
#endif // RAW_OUTPUT
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fprintf(stderr, "\n");
}

void e_exit(const char* fmt, ...) {
#ifdef RAW_OUTPUT
	fprintf(stderr, "F:");
#else
	fprintf(stderr, RED "Fatal: " RESET);
#endif // RAW_OUTPUT
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fprintf(stderr, "\n");
	exit(1);
}

#ifdef _WIN32
#include <Windows.h>
void SetupTerminal(void)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
}
#else
void SetupTerminal(void)
{}
#endif // _WIN32