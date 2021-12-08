#include "XString.h"
#include <string.h>
#include <malloc.h>
/*!
 *  Returns last char of string
 *
 *  @param self pointer to string
 *  @return pointer to last char
 */
static char* last(String* self)
{
	return &self->short_str[sizeof(*self) - 1];
}
/*!
 *  Returns last char of string
 *
 *  @param self pointer to string
 *  @return const pointer to last char
 */
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
		return Newsize + 1; // geometric growth would be insufficient
	}

	return Geometric; // geometric growth is sufficient
}
static void reallocate_for(String* self, size_t sz)
{
	const size_t Oldsize = length_str(self);
	const size_t Newsize = Oldsize + sz;

	if (Newsize <= capacity_str(self))return;
	const size_t Newcapacity = calculate_growth(capacity_str(self), Newsize);

	if (self->is_large)
	{
		self->str = (char*)realloc(self->str, Newcapacity);
		self->al_sz = Newcapacity - 1;
		return;
	}

	char* Replacement = (char*)malloc(Newcapacity);
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
	self->str = (char*)malloc(sz + 1);
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

char at_str(const String* self, size_t pos)
{
	if (pos > length_str(self))return '\0';
	if (self->is_large)
		return self->str[pos];
	return self->short_str[pos];
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

//add character to end of string
void push_back_str(String* self, char c)
{
	if (!self->is_large && *last_c(self))
	{
		self->short_str[sizeof(String) - 1 - (size_t)self->term--] = c;
		self->short_str[sizeof(String) - 1 - (size_t)self->term] = '\0';
		return;
	}

	reallocate_for(self, 1);
	self->str[self->len++] = c;
	self->str[self->len] = '\0';
}

//sets lenght to 0, but doesn't free string
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

void append_str(String* self, const char* input)
{
	StringView x = { input, strlen(input) };
	reallocate_for(self, x.len);

	if (!self->is_large)
	{
		while (*input)
			self->short_str[sizeof(String) - 1 - self->term--] = *input++;
		self->short_str[sizeof(String) - 1 - self->term] = '\0';
		return;
	}
	memcpy(self->str + self->len, x.data, x.len);
	self->len += x.len;
	self->str[self->len] = '\0';
}

void prepend_str(String* self, const char* input)
{
	StringView x = { input, strlen(input) };
	reallocate_for(self, x.len);

	if (!self->is_large)
	{
		memmove(self->short_str + x.len, self->short_str, x.len);
		memcpy(self->short_str, x.data, x.len);
		self->term -= x.len;
		return;
	}
	memmove(self->str + x.len, self->str, x.len);
	memcpy(self->str, x.data, x.len);
	self->len += x.len;
}

void pop_front_str(String* self)
{
	if (!self->is_large)
	{
		memmove(self->short_str, self->short_str + 1, sizeof(String) - 1 - self->term - 1);
		self->short_str[sizeof(String) - 1 - self->term - 1] = '\0';
		self->term++;
		return;
	}
	memmove(self->str, self->str + 1, --self->len);
	self->str[self->len--] = 0;
}

StringView substr_b(const String* self, size_t offset_back)
{
	size_t len = length_str(self);
	if (offset_back > len)return (StringView) { "", 0 };
	return (StringView) { .data = c_str(self) + len - offset_back, .len = offset_back};
}


