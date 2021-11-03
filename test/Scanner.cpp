#include "gtest/gtest.h"
#include <fstream>
#include <string>
#include <iostream>

extern "C"
{
#include "scanner.h"
}

class Scan
{
private:
	Scanner s;
public:
	Scan(FILE* fp)
	{
		Scanner_ctor(&s, fp);
	}
	~Scan()
	{
		Scanner_dtor(&s);
	}
	Error get_token(token& tk)
	{
		return _get_token(&s, &tk);
	}
};

class ProgramTest : public ::testing::Test
{
private:
	Scanner sc;
	Error e;
	std::string current_out;
	//state of test setup, has nothing to do with actual testing
	int state = 0;

private:
	void SetUp() override
	{
		e = Ok;
		current_out = "";
	}
	void TearDown() override
	{
		Scanner_dtor(&sc);
	}

public:
	void SetInput(const char* sourcePath)
	{
		EXPECT_EQ(state, 0);
		if (state != 0)
			e_exit("Wrong calling order in tests! %i!=0", state);

		FILE* fp = fopen(sourcePath, "r");
		Scanner_ctor(&sc, fp);

		state++;
	}
	void RunScanner()
	{
		EXPECT_EQ(state, 1);
		if (state != 1)
			e_exit("Wrong calling order in tests! %i!=1", state);

		//record text in stderr
		testing::internal::CaptureStderr();

		Scanner_print(&sc, &e);

		//saves log to string
		current_out = testing::internal::GetCapturedStderr();

		state++;
	}

	void CheckFolders(const char* TestPathO, const char* TestPathR)
	{
		EXPECT_EQ(state, 2);
		if (state != 2)
			e_exit("Wrong calling order in tests! %i!=2", state);

		//if folders doesn't exists, creates them
		_mkdir(TestPathO);
		_mkdir(TestPathR);

		state++;
	}

	void TestOutput(const char* TestPathOut, const char* TestPathCor)
	{
		EXPECT_EQ(state, 3);
		if (state != 3)
			e_exit("Wrong calling order in tests! %i!=3", state);

		//saves current output to file
		std::ofstream current(TestPathOut);
		current << current_out;
		current.close();

		//Checks if scanner is at end of file
		ASSERT_EQ(e,e_eof);

		//writes rule file to folder test/o if file doesn't exists
		//but only if error didn't occurred
		std::ifstream correct_check(TestPathCor);
		if (!correct_check.good())
		{
			w_msg("New rule file will be generated! Check if its correct.");
			std::ofstream correct(TestPathCor);
			correct << current_out;
			correct.close();
			//waits for confirm
			d_msg("Type \"y\" to confirm and continue or type \"n\" to abort action.");
			char c;
			bool key = true;
			while(key) {
				c = fgetc(stdin);
				switch (c)
				{
				case 'y':
				case 'Y':
					key = false;
					break;
				case 'n':
				case 'N':
					remove(TestPathCor);
					key = false;
					break;
				default:
					break;
				}
			}
		}

		//reads rule file to string
		std::ifstream correct(TestPathCor);
		std::string correct_out((std::istreambuf_iterator<char>(correct)),
							     std::istreambuf_iterator<char>());
		correct.close();

		//checks rule file string and output string
		ASSERT_EQ(correct_out, current_out);

		state++;
	}
};

//test scanner on example files

#define NAME "scan_1.tl"
TEST_F(ProgramTest, scan_1)
{
	const char* sourcePath = SOURCE_DIR "/examples/" NAME;
	SetInput(sourcePath);
	RunScanner();
	const char* TestPathO = SOURCE_DIR"/test/o/";
	const char* TestPathR = SOURCE_DIR"/test/r/";
	CheckFolders(TestPathO, TestPathR);
	const char* TestPathOut = SOURCE_DIR"/test/o/" NAME;
	const char* TestPathCor = SOURCE_DIR"/test/r/" NAME;
	TestOutput(TestPathOut, TestPathCor);
}
#undef NAME

#define NAME "prg_1.tl"
TEST_F(ProgramTest, prg_1)
{
	const char* sourcePath = SOURCE_DIR "/examples/" NAME;
	SetInput(sourcePath);
	RunScanner();
	const char* TestPathO = SOURCE_DIR"/test/o/";
	const char* TestPathR = SOURCE_DIR"/test/r/";
	CheckFolders(TestPathO, TestPathR);
	const char* TestPathOut = SOURCE_DIR"/test/o/" NAME;
	const char* TestPathCor = SOURCE_DIR"/test/r/" NAME;
	TestOutput(TestPathOut, TestPathCor);
}
#undef NAME

#define NAME "prg_2.tl"
TEST_F(ProgramTest, prg_2)
{
	const char* sourcePath = SOURCE_DIR "/examples/" NAME;
	SetInput(sourcePath);
	RunScanner();
	const char* TestPathO = SOURCE_DIR"/test/o/";
	const char* TestPathR = SOURCE_DIR"/test/r/";
	CheckFolders(TestPathO, TestPathR);
	const char* TestPathOut = SOURCE_DIR"/test/o/" NAME;
	const char* TestPathCor = SOURCE_DIR"/test/r/" NAME;
	TestOutput(TestPathOut, TestPathCor);
}
#undef NAME

#define NAME "prg_3.tl"
TEST_F(ProgramTest, prg_3)
{
	const char* sourcePath = SOURCE_DIR "/examples/" NAME;
	SetInput(sourcePath);
	RunScanner();
	const char* TestPathO = SOURCE_DIR"/test/o/";
	const char* TestPathR = SOURCE_DIR"/test/r/";
	CheckFolders(TestPathO, TestPathR);
	const char* TestPathOut = SOURCE_DIR"/test/o/" NAME;
	const char* TestPathCor = SOURCE_DIR"/test/r/" NAME;
	TestOutput(TestPathOut, TestPathCor);
}
#undef NAME