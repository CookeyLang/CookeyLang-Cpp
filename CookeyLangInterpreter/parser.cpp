#include "parser.h"

// macros
#define VALID (i < tokens.size())

// functions
static bool bmatch(std::vector<Token> tokens, int i, TType first);

template <typename... T>
static bool bmatch(std::vector<Token> tokens, int i, TType first, T... other);

static Token badvance(std::vector<Token> tokens, int* i);
static Token bprevious(std::vector<Token> tokens, int i);


void parser(std::vector<Token> tokens, std::string file)
{
	using namespace std::placeholders;

	int i = 0;
	auto match = std::bind_front(bmatch, i, tokens);
	auto advance = std::bind(badvance, tokens, &i);
	auto previous = std::bind(bprevious, tokens, i);
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

static Token badvance(std::vector<Token> tokens, int* i)
{
	if (*i < tokens.size()) *(i)++;
	return bprevious(tokens, *i);
}

static Token bprevious(std::vector<Token> tokens, int i)
{
	return tokens[i - 1];
}

#undef VALID