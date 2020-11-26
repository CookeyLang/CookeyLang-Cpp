#include "parser.h"

// macros
#define VALID (i < tokens.size())

// functions
static bool bmatch(std::vector<Token> tokens, int i, TType first);

template <typename... T>
static bool bmatch(std::vector<Token> tokens, int i, TType first, T... other);


void parser(std::vector<Token> tokens, std::string file)
{
	using namespace std::placeholders;

	int i = 0;
	auto match = std::bind_front(bmatch, i, tokens);
}


static bool bmatch(std::vector<Token> tokens, int i, TType first)
{
	if (!VALID)
		return false;
	return tokens[i].type == first;
}

template <typename... T>
static bool bmatch(std::vector<Token> tokens, int i, TType first, T... other)
{
	if (bmatch(tokens, i, first))
		return true;
	return bmatch(other...);
}

#undef VALID