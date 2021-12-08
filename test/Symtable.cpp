#include "gtest/gtest.h"

extern "C"
{
#include "symtable.h"
#include "Templates.h"
}

class Csymtable
{
private:
	HashMap(FunctionDecl) self;
public:
	Csymtable()
	{
		htab_FunctionDecl_ctor(&self);
	}
	Csymtable(char* a, char* b)
	{
		htab_FunctionDecl_ctor(&self);
		emplace_htab_FunctionDecl(&self, a);
		emplace_htab_FunctionDecl(&self, b);
	}
	~Csymtable()
	{
		htab_FunctionDecl_ctor(&self);
	}
public:
	FunctionDecl* symtable_emplace(const char* id)
	{
		return emplace_htab_FunctionDecl(&self, id);
	}

	FunctionDecl* symtable_find(const char* id)
	{
		return find_htab_FunctionDecl(&self, id);
	}
};

TEST(Csymtable, hash) {
	FunctionDecl *a, *b;

	{
		Csymtable s;
		a = s.symtable_emplace("a");
		b = s.symtable_emplace("b");
		EXPECT_NE(a, (FunctionDecl*) NULL);
		ASSERT_NE(b, (FunctionDecl*) NULL);
		EXPECT_NE(a, b);
	}

	{
		Csymtable s;
		a = s.symtable_emplace("a");
		b = s.symtable_emplace("a");
		EXPECT_NE(a, (FunctionDecl*) NULL);
		ASSERT_EQ(b, (FunctionDecl*) NULL);
		EXPECT_NE(a, b);
	}
}

TEST(Csymtable, fill) {
	FunctionDecl *a, *b;

	{
		Csymtable s{ "a", "b" };
		a = s.symtable_find("a");
		b = s.symtable_find("b");
		EXPECT_NE(a, (FunctionDecl*)NULL);
		ASSERT_NE(b, (FunctionDecl*)NULL);
		EXPECT_NE(a, b);
	}

	{
		Csymtable s{ "a","b" };
		a = s.symtable_find("a");
		b = s.symtable_find("a");
		EXPECT_NE(a, (FunctionDecl*)NULL);
		ASSERT_NE(b, (FunctionDecl*)NULL);
		EXPECT_EQ(a, b);
	}
}