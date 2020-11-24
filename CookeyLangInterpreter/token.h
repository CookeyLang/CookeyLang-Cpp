#ifndef TOKEN_H
#define TOKEN_H

// this is the 'global' file as all stages require it.
#include <string>

#define error(line, col, message, ...) printf("[%d : %d] " message "\n", line, col, ##__VA_ARGS__);

enum class TType
{
	// reserved
	VAR, FINAL, DELETEVARIABLE,
	FUNCTION, RET, EXIT, LAMBDA,
	CLASS, THIS, EXTENDS, SUPERCLASS,
	NAV, TRUE, FALSE,
	IF, EL,
	AND, OR,
	FOREACH, FOR, FORREP, IN, WHILE, BREAK, DO,
	SWITCH, CASE, DEFAULT,

	// literals
	NUMBER, IDENTIFIER, STRING,

	// operators
	PLUS_EQ, PLUS_PLUS, PLUS,
	MINUS_EQ, MINUS_MINUS, MINUS,
	TIMES_EQ, TIMES,
	DIVIDE_EQ, DIVIDE,
	POWER_EQ, POWER,
	MODULO_EQ, MODULO,
	BANG_EQ, BANG,
	EQ_EQ, EQ,
	GREATER_EQ, GREATER,
	LESS_EQ, LESS,

	// symbols
	SEMI, COMMA, DOT, AT,
	LEFT_PAREN, RIGHT_PAREN, LEFT_BRACK, RIGHT_BRACK, LEFT_BRACE, RIGHT_BRACE,
	QUE, COL,

	// end of file
	END
};

class Token
{
public:
	int line, col;
	TType type;
	std::string value;

	Token(int line, int col, TType type, std::string value);
};

#endif