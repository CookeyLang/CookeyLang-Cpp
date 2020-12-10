#include "parser.h"

// macros
#define VALID (i < tokens.size())
#define PEEK (int64_t) i + 1
#define consume(tok, err) {\
int line = tokens[i].line, col = tokens[i].col;\
if (match(tok)) advance();\
else error(err);\
}

Parser::Parser(std::vector<Token> tokens, std::string file)
{
	this->tokens = tokens;
	this->file = file;
}


Expr* Parser::addition()
{
	Expr* expr = multiplication();
	while (match(TType::PLUS, TType::MINUS))
	{
		Token op = previous();
		Expr* right = multiplication();
		expr = new Binary(expr, op, right);
	}

	return expr;
}

Expr* Parser::multiplication()
{
	// todo: unary!
	Expr* expr = primary();
	while (match(TType::DIVIDE, TType::TIMES))
	{
		Token op = previous();
		Expr* right = primary();
		expr = new Binary(expr, op, right);
	}

	return expr;
}

Expr* Parser::unary()
{
	if (match(TType::BANG, TType::MINUS))
	{
		Token op = previous();
		Expr* right = unary();
		return new Unary(op, right);
	}

	return primary();
}

Expr* Parser::primary()
{
	if (match(TType::TRUE)) return new Literal(previous(), true);
	if (match(TType::FALSE)) return new Literal(previous(), false);
	if (match(TType::NAV)) return new Literal(previous(), nullptr);

	if (match(TType::STRING, TType::NUMBER))
	{
		Token prev = previous(); // so we calculate only once
		TType type = prev.type;
		std::string val = prev.value;

		if (type == TType::STRING) return new Literal(prev, val);
		else return new Literal(prev, std::stoi(val));
	}

	if (match(TType::LEFT_PAREN))
	{
		Expr* expr = addition();
		consume(TType::RIGHT_PAREN, "Expected ')' after expression.");
		return new Grouping(expr);
	}
}


Expr* Parser::init()
{
	return addition();
}


// functions
template <typename... T>
bool Parser::match(TType first, T... other)
{
	return match(first) ? true : match(other...);
}

bool Parser::match(TType first)
{
	if (VALID && tokens[i].type == first)
	{
		advance();
		return true;
	}

	return false;
}

Token Parser::advance()
{
	if VALID i++; // lol
	return previous();
}

Token Parser::previous()
{
	return tokens[(int64_t)i - 1];
}

#undef VALID