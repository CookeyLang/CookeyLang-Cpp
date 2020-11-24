#include "lexer.h"

// macros
#define VALID i < code.length()

// functions
static void bappend(std::vector<Token>& output, int line, int col, TType type, std::string val = "");
static void bnewline(int& line, int& col);


std::vector<Token> lexer(std::string code)
{
	static int line = 1, col = 1, i = 0;
	static std::vector<Token> output;

	using namespace std::placeholders;
	auto append = std::bind(bappend, output, line, col, _1, _2);

	while (VALID) {
		char curr = code[i];
	}

	return output;
};


static void bappend(std::vector<Token>& output, int line, int col, TType type, std::string val = "")
{
	output.push_back(Token(line, col, type, val));
}

static void bnewline(int& line, int& col)
{

}

#undef VALID