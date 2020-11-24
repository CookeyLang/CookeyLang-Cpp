#include "lexer.h"

// macros
#define VALID i < code.length()

// functions
static void bappend(std::vector<Token>& output, int line, int col, TType type, std::string val = "");

static void bnewline(int& line, int& col, int& i);
static void bnext(int& col, int& i);

static bool bmatch(std::string code, int& i, char c);

static bool isNum(char c);
static bool isAlpha(char c);
static bool isAlphaNum(char c);


std::vector<Token> lexer(std::string code)
{
	int line = 1, col = 1, i = 0;
	std::vector<Token> output;

	using namespace std::placeholders; // _1, _2, etc
	auto append = std::bind(bappend, output, line, col, _1, _2);
	auto newline = std::bind(bnewline, line, col, i);
	auto next = std::bind(bnext, col, i);
	auto match = std::bind(bmatch, code, i, _1);

	while (VALID) {
		char curr = code[i];
	}

	return output;
};


static void bappend(std::vector<Token>& output, int line, int col, TType type, std::string val)
{
	output.push_back(Token(line, col, type, val));
}

static void bnewline(int& line, int& col, int& i)
{
	line++;
	col = 1;
	i++;
}

static void bnext(int& col, int& i)
{
	col++;
	i++;
}

static bool isNum(char c)
{
	return c >= '0' && c <= '9';
}

static bool isAlpha(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool isAlphaNum(char c)
{
	return isAlpha(c) || isNum(c);
}

static bool bmatch(std::string code, int& i, char c)
{
	if (code[(int64_t)i + 1] == c) {
		i++;
		return true;
	}
	return false;
}

#undef VALID