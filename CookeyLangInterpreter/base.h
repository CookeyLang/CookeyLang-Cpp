#ifndef P_BASE_H
#define P_BASE_H

#include <string>
#include "token.h"

class Expr
{
public:
	int line, col;
	std::string file;

	Expr() { line = 0, col = 0, file = ""; }
};

#endif