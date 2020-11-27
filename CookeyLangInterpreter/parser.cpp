#include "parser.h"

// macros
#define VALID (i < tokens.size())

Parser::Parser(std::vector<Token> tokens, std::string file)
{
	this->tokens = tokens;
	this->file = file;
}


Expr Parser::init()
{
	// todo :(
	return Expr();
}


// functions
template <typename... T>
bool Parser::match(TType first, T... other)
{
	return match(first) ? true : match(other...);
}

bool Parser::match(TType first)
{
	if (VALID && tokens[i].type == first)
	{
		advance();
		return true;
	}

	return false;
}

Token Parser::advance()
{
	if VALID i++; // lol
	return previous();
}

Token Parser::previous()
{
	return tokens[(int64_t)i - 1];
}

#undef VALID