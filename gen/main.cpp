#include <string>
#include <fstream>
#include <vector>
#include <utility>
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <format>
#include <iomanip>

using namespace std;

auto const max_print = 3;
auto const separation = 1;

namespace mystd {
	//stores words
	typedef vector<string> cell;
	//stores cells in column and name of column
	typedef pair<string, vector<cell>> column;
	//stores column
	typedef vector<column> table;
}

mystd::table read_csv(string filename, char separator) {
	mystd::table result;
	ifstream myFile(filename);

	if (!myFile.is_open()) throw runtime_error("Could not open " + filename + " file");

	string line;

	//load header to table
	if (myFile.good())
	{
		getline(myFile, line);
		stringstream ss(line);
		string colname;
		while (getline(ss, colname, separator)) {
			if (colname.starts_with('\"')) {
				string colname_sub = colname.substr(3, colname.size() - 6);
				result.push_back({ colname_sub, vector<mystd::cell> {} });
			}
			else {
				result.push_back({ colname, vector<mystd::cell> {} });
			}
		}
	}

	// Read line by line
	while (getline(myFile, line))
	{
		//end of file or empty file
		if (line.empty()) break;
		//lower half of table, we don't care about
		if (line.starts_with('\"')) break;
		stringstream stream_line(line);

		int colIdx = 0;
		string column;
		while (getline(stream_line, column, separator)) {
			mystd::cell cell = {};

			if (!column.empty()) {
				if (column.starts_with('\"')) {
					//cell contains cell
					//removes first " and then skips rule number in next cycle
					string column_sub = column.substr(1, column.size());
					stringstream stream_column(column_sub);
					string word;
					while (getline(stream_column, word, ' ')) {

						if (word.starts_with('\"')) {
							//cell ""name""
							string word_sub = word.substr(2, word.size() - 4);
							//removes last and first ""
							//just name
							if (word.ends_with(',')) {
								//format: word, rule number
								//removes ',' and skips rule number
								cell.push_back(word_sub.substr(0, word_sub.size() - 1));
								break;
							}
							else {
								//just name
								cell.push_back(word_sub);
							}
						}
						else if (word.ends_with(',')) {
							cell.push_back(word.substr(0, word.size() - 1));
							break;
						}
						else {
							cell.push_back(word);
						}
					}
				}
				else {
					//doens't contain cell
					stringstream stream_column(column);
					string word;
					while (getline(stream_column, word, ' ')) {
						if (word.ends_with(',')) {
							cell.push_back(word.substr(0, word.size() - 1));
							break;
						}
						else {
							cell.push_back(word);
						}
					}
				}
			}
			result.at(colIdx).second.push_back(cell);
			colIdx++;
		}
	}

	myFile.close();
	return result;
}

void write_head(ofstream* output) {
	*output << "\n";
	*output << "#define T(n) (token_type)(T0+n)\n";
	*output << "#define PUSH  *push_back_Vector_token_type(stack) = \n";
	*output << "\n";
	*output << "typedef Error(*Table)(Vector(token_type)* stack, token_type input_tt);\n";
	*output << "\n";
}

void  write_switch(ofstream* output, mystd::column column) {
	//stores epsilon rules
	vector<int> empty;

	for (auto line_n = 0; line_n < column.second.size(); line_n++)
	{
		mystd::cell cell = column.second.at(line_n);
		if (cell.empty())
			continue;

		else if (cell.at(0) == "epsilon") {
			empty.push_back(line_n);
			continue;
		}

		*output << "\tcase T(" << line_n << "):\n";
		for (long long cmd_n = cell.size() - 1; cmd_n >= 0; cmd_n--)
		{
			if (cell.at(cmd_n).starts_with('T')) {
				*output << "\t\tPUSH T(" << cell.at(cmd_n).substr(1) << ");\n";
			}
			else {
				*output << "\t\tPUSH tt_" << cell.at(cmd_n) << ";\n";
			}
		}
		*output << "\t\tbreak;\n";
	}

	if (empty.size() > 0) {
		for (auto count = 0; count < empty.size(); count++) {
			*output << "\tcase T(" << empty.at(count) << "):\n";
		}
		*output << "\t\tbreak;\n";
	}
}

void write_body(ofstream* output, mystd::table table) {
	for (auto i = 0; i < table.size(); i++)
	{
		*output << "Error " << table.at(i).first << "_f(Vector(token_type)* stack, token_type top_tt)\n";
		*output << "{\n";
		*output << "\tswitch ((int)top_tt)\n";
		*output << "\t{\n";

		write_switch(output, table.at(i));

		*output << "\tdefault:\n";
		*output << "\t\treturn e_invalid_syntax;\n";
		*output << "\t}\n";
		*output << "\treturn e_ok;\n";
		*output << "}\n";
	}
}

void write_end(ofstream* output, mystd::table table) {
	*output << "\n";
	*output << "Error LLTable(Vector(token_type)* stack, token_type input_tt, token_type top_tt)\n";
	*output << "{\n";
	*output << "\tswitch(input_tt)\n";
	*output << "\t{\n";

	for (auto i = 0; i < table.size(); i++) {
		*output << "\tcase tt_" << table.at(i).first << ":";
		*output << setw(30 - table.at(i).first.size());
		*output << "return " << table.at(i).first << "_f(stack, top_tt);\n";
	}
	string default_case = "\tdefault:";
	*output << default_case;
	*output << setw(30 - default_case.size());
	*output << "return e_invalid_syntax;\n";

	*output << "\t}\n";
	*output << "}\n";
	*output << "\n";
	*output << "#undef T\n";
	*output << "#undef PUSH\n";
}

//rewrites names
void normalize(string* string) {
	if (*string == "STRLIT")
		*string = "string_literal";
	if (*string == "IDENTIFIER")
		*string = "identifier";
	else if (*string == "(")
		*string = "left_parenthese";
	else if (*string == ")")
		*string = "right_parenthese";
	else if (*string == ":")
		*string = "colon";
	else if (*string == ",")
		*string = "comma";
	else if (*string == "TYPE")
		*string = "type";
	else if (*string == "=")
		*string = "assign";
	else if (*string == "EXPRESSION")
		*string = "expression";
	else if (*string == "Îµ" || *string == "ε")
		*string = "epsilon";
	else if (*string == "$")
		*string = "eof";
}

//rewrites all names in table
void convert_table(mystd::table* table) {
	for (auto col_n = 0; col_n < (*table).size(); col_n++)
	{
		mystd::column* column = &((*table).at(col_n));
		for (auto line_n = 0; line_n < (*column).second.size(); line_n++)
		{
			mystd::cell* cell = &((*column).second.at(line_n));
			for (auto data_n = 0; data_n < (*cell).size(); data_n++)
			{
				normalize(&((*cell).at(data_n)));
			}
			normalize(&((*column).first));
		}
	}
}

void print_table(ofstream* log_file, mystd::table table) {
	//with of columns
	vector<size_t> space;
	//text
	vector<vector<string>> text;

	//gets heading of table to text
	vector<string> line;
	for (size_t col_n = 0; col_n < table.size(); col_n++)
	{
		string word = table.at(col_n).first;
		line.push_back(word.substr(0, max_print));
	}
	text.push_back(line);

	//gets all words to text line by line
	for (size_t line_n = 0; line_n < table.at(0).second.size(); line_n++)
	{
		line = {};
		for (size_t col_n = 0; col_n < table.size(); col_n++)
		{
			string cell_text;
			mystd::cell cell = table.at(col_n).second.at(line_n);
			for (size_t word_n = 0; word_n < cell.size(); word_n++)
				if (cell.size() - word_n > 1)
					cell_text += cell.at(word_n).substr(0, max_print) + ' ';
				else
					cell_text += cell.at(word_n).substr(0, max_print);
			line.push_back(cell_text);
		}
		text.push_back(line);
	}

	//calculates spaces
	for (size_t col_n = 0; col_n < text.at(0).size(); col_n++) {
		size_t max = 0;
		for (size_t line_n = 0; line_n < text.size(); line_n++) {
			size_t count = separation;
			count += text.at(line_n).at(col_n).size();
			if (max < count)
				max = count;
		}
		space.push_back(max);
	}

	//prints formated table
	///@note edit formating here
	for (size_t line_n = 0; line_n < table.at(0).second.size(); line_n++)
	{
		*log_file << '#';
		line = {};
		for (size_t col_n = 0; col_n < table.size(); col_n++)
		{
			///@note adds column separator
			if (col_n != 0)
				*log_file << '|';
			string word = text.at(line_n).at(col_n);
			*log_file << setw(space.at(col_n));
			*log_file << left << word;
		}
		*log_file << "#\n";
	}
}

int main(int argc, char** argv) {
	mystd::table table = read_csv(SOURCE_DIR"/gen/ept.csv", ';');

	convert_table(&table);

	ofstream log_file(SOURCE_DIR"/gen/out.txt");
	print_table(&log_file, table);
	log_file.close();

	table.erase(table.begin(), table.begin() + 2);	//erase head of table and Starting token
	table.erase(table.begin() + 1);					//erase STRING LITERAL

	ofstream inl_file(SOURCE_DIR"/include/Grammar.inl");
	write_head(&inl_file);
	write_body(&inl_file, table);
	write_end(&inl_file, table);
	inl_file.close();

	return 0;
}