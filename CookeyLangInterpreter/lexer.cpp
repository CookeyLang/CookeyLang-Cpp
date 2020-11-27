#include "lexer.h"

// macros
#define VALID i < code.length()
#define PEEK (int64_t) i + 1

Lexer::Lexer(std::string code, std::string file)
{
	this->code = code;
	this->file = file;

	reserved = {
		// Variables
		{ "var", TType::VAR }, { "final", TType::FINAL }, { "deleteVariable", TType::DELETEVARIABLE },

		// Functions
		{ "function", TType::FUNCTION }, { "ret", TType::RET }, { "exit", TType::EXIT }, { "lambda", TType::LAMBDA },

		// Classes
		{ "class", TType::CLASS }, { "this", TType::THIS }, { "extends", TType::EXTENDS }, { "superClass", TType::SUPERCLASS },

		// Values
		{ "NaV", TType::NAV }, { "true", TType::TRUE }, { "false", TType::FALSE },

		// If Statements
		{ "if", TType::IF }, { "el", TType::EL },

		// Logic
		{ "and", TType::AND }, { "or", TType::OR },

		// Loops
		{ "foreach", TType::FOREACH }, { "for", TType::FOR }, { "forrep", TType::FORREP }, { "in", TType::IN }, { "while", TType::WHILE }, { "break", TType::BREAK }, { "do", TType::DO },

		// Switch
		{ "switch", TType::SWITCH }, { "case", TType::CASE }, { "default", TType::DEFAULT }
	};
}

std::vector<Token> Lexer::init()
{
	while (VALID)
	{
		char curr = code[i];

		if (isNum(curr))
		{
			std::string value;
			value += curr;

			while (isNum(code[PEEK]))
			{
				next();
				value += code[i];
			}

			if (code[PEEK] == '.' && isNum(code[PEEK + 1]))
			{
				next(); // the .
				value += code[i];

				while (isNum(code[PEEK]))
				{
					next();
					value += code[i];
				}
			}

			append(TType::NUMBER, value);
		}
		else if (isAlpha(curr))
		{
			std::string value;
			value += curr;

			while (isAlphaNum(code[PEEK]))
			{
				next();
				value += code[i];
			}

			if (reserved.count(value))
				append(reserved[value], value);
			else
				append(TType::IDENTIFIER, value);
		}
		else
			switch (curr)
			{
			case '+':
				match('=') ? append(TType::PLUS_EQ) : match('+') ? append(TType::PLUS_PLUS) : append(TType::PLUS);
				break;
			case '-':
				match('=') ? append(TType::MINUS_EQ) : match('-') ? append(TType::MINUS_MINUS) : append(TType::MINUS);
				break;
			case '*':
				match('=') ? append(TType::TIMES_EQ) : append(TType::TIMES);
				break;
			case '/':
				match('=') ? append(TType::DIVIDE_EQ) : append(TType::DIVIDE);
				break;
			case '^':
				match('=') ? append(TType::POWER_EQ) : append(TType::POWER);
				break;

			case '%':
				if (match('%'))
					while (VALID && code[PEEK] != '\n')
						i++;
				else if (match('*'))
				{
					while (VALID && !(code[PEEK] == '*' && code[(int64_t)i + 2] == '%'))
						code[i] == '\n' ? newline() : next();

					if (code[PEEK] != '*' && code[(int64_t)i + 2] != '%')
					{
						error("Unterminated multi-line comment.");
					}
					else
					{
						next(); // the *
						next(); // the %
					}
				}
				else if (match('='))
					append(TType::MODULO_EQ);
				else
					append(TType::MODULO);
				break;

			case ';':
				append(TType::SEMI);
				break;
			case ',':
				append(TType::COMMA);
				break;
			case '.':
				append(TType::DOT);
				break;
			case '@':
				append(TType::AT);
				break;

			case '(':
				append(TType::LEFT_PAREN);
				break;
			case ')':
				append(TType::RIGHT_PAREN);
				break;
			case '{':
				append(TType::LEFT_BRACE);
				break;
			case '}':
				append(TType::RIGHT_BRACE);
				break;

			case '!':
				match('=') ? append(TType::BANG_EQ) : append(TType::BANG);
				break;
			case '=':
				match('=') ? append(TType::EQ_EQ) : append(TType::EQ);
				break;

			case '>':
				match('=') ? append(TType::GREATER_EQ) : append(TType::GREATER);
				break;
			case '<':
				match('=') ? append(TType::LESS_EQ) : append(TType::LESS);
				break;

			case '?':
				append(TType::QUE);
				break;
			case ':':
				append(TType::COL);
				break;

			case '"':
			case '\'': {
				char type = curr;
				std::string value;

				while (VALID && code[PEEK] != type)
				{
					next();

					if (code[i] == '\\')
					{
						next(); // the \ escape
						switch (code[i])
						{
						case '\'':
							value += "'";
							break;

						case '"':
							value += '"';
							break;

						case 'r':
							value += '\r';
							break;

						case 'n':
							value += '\n';
							break;

						case 'm': {

							std::string keycode;

							while (VALID && isNum(code[PEEK]))
							{
								next();
								keycode += code[i];
							}

							value += (char)(std::stoi(keycode));
						}
						break;

						case 'u': {

							if (code[PEEK] != '{')
								error( "Expected a '{' after unicode escape sequence.");
							next(); // the {

							std::string hex;

							while (code[PEEK] != '}')
							{
								next();
								hex += code[i];
							}

							if (code[PEEK] != '}')
								error("Expect '}' after unicode escape code.");
							next();

							value += (char)(std::stoi(hex, nullptr, 16)); // todo
						}
						break;

						case 'e':
							value += '\x1b';
							break;

						case '0':
							value += '\0';
							break;

						case '\\':
							value += '\\';
							break;

						default:
							error("Unrecognized escape sequence %c", code[i]);
							break;
						}
					}

					value += code[i];
				}

				if (code[PEEK] != type)
				{
					error("Unterminated string");
				}
				else
					next(); // the " or '

				append(TType::STRING, value);
			}
			break;

			case ' ':
			case '\r':
			case '\t':
				// ignore whitespace
				break;

			case '\n':
				newline();
				continue; // avoid skipping next char

			default:
				error("Unexpected character %c", curr);
				break;
			}

		next();
	}

	return output;
};


// functions
void Lexer::append(TType type, std::string val)
{
	output.push_back(Token(line, col, file, type, val));
}

void Lexer::newline()
{
	line++;
	col = 1;
	i++;
}

void Lexer::next()
{
	col++;
	i++;
}

bool Lexer::isNum(char c)
{
	return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::isAlphaNum(char c)
{
	return isAlpha(c) || isNum(c);
}

bool Lexer::match(char c)
{
	if (code[(int64_t)i + 1] == c)
	{
		i++;
		return true;
	}

	return false;
}

/*
static void bappend(std::vector<Token>* output, int line, int col, std::string file, TType type, std::string val)
{
	output->push_back(Token(line, col, file, type, val));
}

static void bnewline(int* line, int* col, int* i)
{
	(*line)++;
	*col = 1;
	(*i)++;
}

static void bnext(int* col, int* i)
{
	(*col)++;
	(*i)++;
}

static bool isNum(char c)
{
	return c >= '0' && c <= '9';
}

static bool isAlpha(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool isAlphaNum(char c)
{
	return isAlpha(c) || isNum(c);
}

static bool bmatch(std::string code, int* i, char c)
{
	if (code[(int64_t)(*i) + 1] == c)
	{
		(*i)++;
		return true;
	}
	return false;
}
*/

#undef VALID
#undef PEEK