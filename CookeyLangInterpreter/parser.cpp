#include "parser.h"

// macros
#define VALID (i < tokens.size())

// functions
static bool bmatch(std::vector<Token> tokens, int* i, TType first);

template <typename... T>
static bool bmatch(std::vector<Token> tokens, int* i, TType first, T... other);

static Token badvance(std::vector<Token> tokens, int* i);
static Token bprevious(std::vector<Token> tokens, int i);


void parser(std::vector<Token> tokens, std::string file)
{
	using namespace std::placeholders;

	int i = 0;
	auto match = [&](auto... T) { bmatch(tokens, &i, T...); };
	auto advance = std::bind(badvance, tokens, &i);
	auto previous = std::bind(bprevious, tokens, i);

	match(TType::AND);
	// bmatch(tokens, &i, TType::AND);
}


static bool bmatch(std::vector<Token> tokens, int* i, TType first)
{
	if (*i >= tokens.size() && tokens[*i].type == first)
	{
		badvance(tokens, i);
		return true;
	}

	return false;
}

template <typename... T>
static bool bmatch(std::vector<Token> tokens, int* i, TType first, T... other)
{
	if (bmatch(tokens, i, first))
		return true;
	return bmatch(other...);
}

static Token badvance(std::vector<Token> tokens, int* i)
{
	if (*i < tokens.size()) (*i)++;
	return bprevious(tokens, *i);
}

static Token bprevious(std::vector<Token> tokens, int i)
{
	return tokens[(int64_t)i - 1];
}

#undef VALID