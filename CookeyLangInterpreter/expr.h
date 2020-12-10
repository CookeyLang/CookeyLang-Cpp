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

	void visit()
	{
		std::cout << value.get();
	}
};

class Binary : public Expr
{
public:
	Expr* left;
	Token op;
	Expr* right;

	Binary(Expr* left, Token op, Expr* right)
	{
		line = op.line;
		col = op.col;
		file = op.file;

		this->left = left;
		this->op = op;
		this->right = right;
	}

	void visit()
	{
		std::cout << '(';

		switch (op.type)
		{
		case TType::PLUS:
			std::cout << '+';
			break;
		case TType::MINUS:
			std::cout << '-';
			break;
		case TType::DIVIDE:
			std::cout << '/';
			break;
		case TType::TIMES:
			std::cout << '*';
			break;
		}

		left->visit();
		right->visit();

		std::cout << ')';
	}
};

class Unary : public Expr
{
public:
	Token op;
	Expr* right;

	Unary(Token op, Expr* right)
	{
		line = op.line;
		col = op.col;
		file = op.file;

		this->op = op;
		this->right = right;
	}

	void visit()
	{
		std::cout << '(';

		switch (op.type)
		{
		case TType::MINUS:
			std::cout << '-';
			break;
		case TType::BANG:
			std::cout << '!';
			break;
		}

		right->visit();
		std::cout << ')';
	}
};

class Grouping : public Expr
{
public:
	Expr* expr;

	Grouping(Expr* expr)
	{
		this->expr = expr;
	}

	void visit()
	{
		std::cout << '(';
		expr->visit();
		std::cout << ')';
	}
};


#endif