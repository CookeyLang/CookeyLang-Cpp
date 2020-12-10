#ifndef P_BASE_H
#define P_BASE_H

#include <string>
#include <iostream> // todo
#include "token.h"

class Expr
{
public:
	int line, col;
	std::string file;

	Expr() { line = 0, col = 0, file = ""; }
	virtual void visit() {} //{ std::cout << "called"; }
};

#endif