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

class Binary : public Expr
{
public:
	Expr left;
	Token op;
	Expr right;

	Binary(Expr left, Token op, Expr right)
	{
		line = op.line;
		col = op.col;
		file = op.file;

		this->left = left;
		this->op = op;
		this->right = right;
	}
};


#endif