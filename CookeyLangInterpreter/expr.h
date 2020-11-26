#ifndef P_EXPR_H
#define P_EXPR_H

#include <memory>
#include "base.h"

// https://devblogs.microsoft.com/cppblog/stdany-how-when-and-why/
class Literal : public Expr
{
public:
	std::shared_ptr<void> value;
	template<typename T>
	Literal(Token token, T value)
	{
		line = token.line;
		col = token.col;
		file = token.file;

		this->value = std::make_shared<T>(value);
	}
};


#endif