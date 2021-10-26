#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

// TODO: keep technique DRY alive
// i didnt know how to pass va_list to function

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define RESET   "\x1b[0m"

//prints debug in stderr
void d_msg(const char* fmt, ...) {
	fprintf(stderr, GREEN "Debug: " RESET);
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fprintf(stderr, "\n");
}

//prints warning in stderr
void w_msg(const char* fmt, ...) {
	fprintf(stderr, YELLOW "Warn:  " RESET);
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fprintf(stderr, "\n");
}

//prints error in stderr
void e_msg(const char* fmt, ...) {
	fprintf(stderr, RED "Error: " RESET);
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fprintf(stderr, "\n");
}

//prints error in stderr
//and shuts down program
void e_exit(const char* fmt, ...) {
	fprintf(stderr, RED "Fatal: " RESET);
	va_list args;
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fprintf(stderr, "\n");
	exit(1);
}

void usage(char* prgName)
{
	printf("Usage: %s [-h] < input_tl_prg > output_ifj21_prg\n", prgName);
}
