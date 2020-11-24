#include "token.h"

Token::Token(int line, int col, TType type, std::string value)
{
	this->line = line;
	this->col = col;
	this->type = type;
	this->value = value;
}