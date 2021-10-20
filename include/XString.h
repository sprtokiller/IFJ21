#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef union String String;

union String
{
	struct
	{
		char* str;
		size_t len;
		size_t al_sz : sizeof(size_t)*8 - 1;
		size_t is_large : 1;
	};
	char short_str[sizeof(char*) + sizeof(size_t)*2];
};

void String_ctor(String* self, const char* str);
void String_dtor(String* self);

const char* c_str(const String* self);

bool empty_str(const String* self);

size_t length_str(const String* self);
void push_back_str(String* self, char c);
