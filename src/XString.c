#include "XString.h"
#include <string.h>
#include <malloc.h>

static char* last(String* self)
{
	return &self->short_str[sizeof(*self) - 1];
}
static const char* last_c(const String* self)
{
	return &self->short_str[sizeof(*self) - 1];
}
static char i_size(size_t sz)
{
	return (char)sizeof(String) - 1 - (char)sz;
}
static size_t capacity_str(const String* self)
{
	if (self->is_large)
		return self->al_sz;
	return sizeof(*self) - 1;
}



static inline size_t max_size()
{
	return 1ull << ((sizeof(size_t) * 8) - 2);
}
static inline size_t calculate_growth(size_t Oldcapacity, size_t Newsize) //MS STL Impl
{
	// given _Oldcapacity and _Newsize, calculate geometric growth
	const size_t Max = max_size();

	if (Oldcapacity > Max - Oldcapacity / 2)
	{
		return Max; // geometric growth would overflow
	}

	const size_t Geometric = Oldcapacity + Oldcapacity / 2;
	if (Geometric < Newsize)
	{
		return Newsize; // geometric growth would be insufficient
	}

	return Geometric; // geometric growth is sufficient
}
static void reallocate_for(String* self, size_t sz)
{
	const size_t Oldsize = length_str(self);
	const size_t Newsize = Oldsize + sz;

	if (Newsize <= capacity_str(self))return;
	const size_t Newcapacity = calculate_growth(Oldsize, Newsize);

	if (self->is_large)
	{
		self->str = realloc(self->str, Newcapacity);
		self->al_sz = Newcapacity - 1;
		return;
	}

	char* Replacement = malloc(Newcapacity);
	memcpy(Replacement, self, Oldsize + 1);

	self->str = Replacement;
	self->len = Oldsize;
	self->al_sz = Newcapacity - 1;
	self->is_large = 1;				//reasoning, 2^64 is not a good string to begin with
}

void String_ctor(String* self, const char* str)
{
	memset(self->short_str, 0, sizeof(*self));
	if (!str) { *last(self) = i_size(0); return; }
	size_t sz = strlen(str);
	if (sz < sizeof * self - 1)
	{
		memcpy(self->short_str, str, sz);
		*last(self) = i_size(sz);
		return;
	}
	self->str = malloc(sz + 1);
	memcpy(self->str, str, sz + 1);
	self->al_sz = sz;
	self->len = sz;
	self->is_large = true;
}

void String_dtor(String* self)
{
	if (self->is_large)
		free(self->str);
}

void String_move_ctor(String* self, String* other)
{
	*self = *other;
	memset(other, 0, sizeof(*other));
	*last(other) = i_size(0);
}

const char* c_str(const String* self)
{
	if (self->is_large)
		return self->str;
	return self->short_str;
}

bool empty_str(const String* self)
{
	if (self->is_large)
		return self->len == 0;
	return !self->short_str[0];
}

size_t length_str(const String* self)
{
	if (self->is_large)
		return self->len;
	return sizeof(String) - 1 - (size_t)*last_c(self);
}

void push_back_str(String* self, char c)
{
	if (!self->is_large && *last_c(self))
	{
		self->short_str[sizeof(String) - 1 - (size_t)self->short_str[sizeof(String) - 1]--] = c;
		self->short_str[sizeof(String) - 1 - (size_t)self->short_str[sizeof(String) - 1]] = '\0';
		return;
	}

	reallocate_for(self, 1);
	self->str[self->len++] = c;
	self->str[self->len] = '\0';
}

void clear_str(String* self)
{
	if (self->is_large)
	{
		self->len = 0;
		self->str[0] = '\0';
		return;
	}
	*last(self) = i_size(0);
	self->short_str[0] = '\0';
}


