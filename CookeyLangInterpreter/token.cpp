#include "token.h"

Token::Token(int line, int col, std::string file, TType type, std::string value)
{
	this->line = line;
	this->col = col;
	this->file = file;
	this->type = type;
	this->value = value;
}