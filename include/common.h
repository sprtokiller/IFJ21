#pragma once

//Ok		running state of scanner
//e_eof		end of file
//e_invalid_token
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
