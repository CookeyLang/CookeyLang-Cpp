#ifndef P_BASE_H
#define P_BASE_H

#include <string>
#include "token.h"

class Expr
{
public:
	int line, col;
	std::string file;

	virtual void visit();
};

#endif