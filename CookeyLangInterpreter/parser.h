#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "expr.h"

#include <vector>

class Parser
{
	int i = 0;
	std::vector<Token> tokens;
	std::string file;

	template <typename... T>
	bool match(TType first, T... other);
	bool match(TType first);

	Token advance();
	Token previous();

	// main 'parsing' functions
	Expr* addition();
	Expr* multiplication();
	Expr* unary();
	Expr* primary();

public:
	Parser(std::vector<Token> tokens, std::string file);
	Expr* init();
};

#endif