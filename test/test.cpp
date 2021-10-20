#include "gtest/gtest.h"

extern "C"
{
#include "XString.h"
}

class WString
{
public:
	WString()
	{
		String_ctor(&s, nullptr);
	}
	WString(const char* str)
	{
		String_ctor(&s, str);
	}
	~WString()
	{
		String_dtor(&s);
	}
public:
	auto c_str()const
	{
		return ::c_str(&s);
	}
	auto empty()const
	{
		return ::empty_str(&s);
	}
	auto length()const
	{
		return ::length_str(&s);
	}
	bool is_long()const
	{
		return s.is_large;
	}
	void push_back(char c)
	{
		push_back_str(&s, c);
	}
private:
	String s;
};

TEST(String, creation) {
	{
		WString s;
		EXPECT_TRUE(s.empty());
		EXPECT_EQ(s.length(), 0);
		EXPECT_FALSE(s.is_long());
	}

	{
		WString s{ "abcd" };
		EXPECT_FALSE(s.empty());
		EXPECT_EQ(s.length(), 4);
		EXPECT_STREQ(s.c_str(), "abcd");
		EXPECT_FALSE(s.is_long());
	}

	{
		std::string a = "abcdefghierdfzsdzsczcsdfzds";
		WString s{ a.c_str() };
		EXPECT_FALSE(s.empty());
		EXPECT_EQ(s.length(), a.size());
		EXPECT_STREQ(s.c_str(), a.c_str());
		EXPECT_TRUE(s.is_long());
	}
}

TEST(String, push_back) {
	String s;
	String_ctor(&s, nullptr);
	push_back_str(&s, 'a');
	EXPECT_FALSE(empty_str(&s));
	EXPECT_NE(length_str(&s), 0);
	EXPECT_STREQ(c_str(&s), "a");
	String_dtor(&s);

	{
		WString s{ "abcd" };
		s.push_back('e');
		EXPECT_EQ(s.length(), 5);
		EXPECT_STREQ(s.c_str(), "abcde");
		s.push_back('f');
		EXPECT_EQ(s.length(), 6);
		EXPECT_STREQ(s.c_str(), "abcdef");
	}

	{
		std::string a = "abcdefghierdfzsdzsczcsdfzds";
		WString s{ a.c_str() };
		s.push_back('e');
		EXPECT_EQ(s.length(), a.size() + 1);
		EXPECT_STREQ(s.c_str(), "abcdefghierdfzsdzsczcsdfzdse");
		s.push_back('f');
		EXPECT_EQ(s.length(), a.size() + 2);
		EXPECT_STREQ(s.c_str(), "abcdefghierdfzsdzsczcsdfzdsef");
	}

	{
		std::string a = "abcdefghierdfzsdzsczcs";
		WString s{ a.c_str() };
		EXPECT_FALSE(s.is_long());
		s.push_back('e');
		EXPECT_EQ(s.length(), a.size() + 1);
		EXPECT_STREQ(s.c_str(), "abcdefghierdfzsdzsczcse");
		EXPECT_FALSE(s.is_long());
		s.push_back('f');
		EXPECT_EQ(s.length(), a.size() + 2);
		EXPECT_STREQ(s.c_str(), "abcdefghierdfzsdzsczcsef");
		EXPECT_TRUE(s.is_long());
	}
}
