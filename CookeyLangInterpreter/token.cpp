#include "token.h"

Token::Token(int line, int col, TType type)
{
	this->line = line;
	this->col = col;
	this->type = type;
}