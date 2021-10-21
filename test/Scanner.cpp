#include "gtest/gtest.h"

extern "C"
{
#include "scanner.h"
}

class Scan
{
public:
	Scan(FILE* fp)
	{
		Scanner_ctor(&s, fp);
	}
	~Scan()
	{
		Scanner_dtor(&s);
	}
public:
	Error get_token(token& tk)
	{
		return _get_token(&s, &tk);
	}
private:
	Scanner s;
};
