#ifndef LEXER_H
#define LEXER_H

#include "lexer.h"
#include "token.h"

#include <vector>
#include <map>

class Lexer
{
	std::vector<Token> output;
	std::map<std::string, TType> reserved;

	int line = 1, col = 1, i = 0;
	std::string file, code;

	void append(TType type, std::string val = "");
	void newline();
	void next();
	bool match(char c);

	bool isNum(char c);
	bool isAlpha(char c);
	bool isAlphaNum(char c);

public:
	Lexer(std::string code, std::string file);
	std::vector<Token> init();
};

#endif