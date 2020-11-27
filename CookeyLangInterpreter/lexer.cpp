#include "lexer.h"

// macros
#define VALID i < code.length()
#define PEEK (int64_t) i + 1

// functions
static void bappend(std::vector<Token>* output, int line, int col, std::string file, TType type, std::string val);

static void bnewline(int* line, int* col, int* i);
static void bnext(int* col, int* i);

static bool bmatch(std::string code, int* i, char c);

static bool isNum(char c);
static bool isAlpha(char c);
static bool isAlphaNum(char c);


std::vector<Token> lexer(std::string code, std::string file)
{
	int line = 1, col = 1, i = 0;
	std::vector<Token> output;
	std::map<std::string, TType> reserved = {
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
	using namespace std::placeholders; // _1, _2, etc


	while (VALID)
	{
		auto append = std::bind(bappend, &output, line, col, file, _1, _2);
		auto apptok = std::bind(bappend, &output, line, col, file, _1, "");
		auto newline = std::bind(bnewline, &line, &col, &i);
		auto next = std::bind(bnext, &col, &i);
		auto match = std::bind(bmatch, code, &i, _1);

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
				match('=') ? apptok(TType::PLUS_EQ) : match('+') ? apptok(TType::PLUS_PLUS) : apptok(TType::PLUS);
				break;
			case '-':
				match('=') ? apptok(TType::MINUS_EQ) : match('-') ? apptok(TType::MINUS_MINUS) : apptok(TType::MINUS);
				break;
			case '*':
				match('=') ? apptok(TType::TIMES_EQ) : apptok(TType::TIMES);
				break;
			case '/':
				match('=') ? apptok(TType::DIVIDE_EQ) : apptok(TType::DIVIDE);
				break;
			case '^':
				match('=') ? apptok(TType::POWER_EQ) : apptok(TType::POWER);
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
					apptok(TType::MODULO_EQ);
				else
					apptok(TType::MODULO);
				break;

			case ';':
				apptok(TType::SEMI);
				break;
			case ',':
				apptok(TType::COMMA);
				break;
			case '.':
				apptok(TType::DOT);
				break;
			case '@':
				apptok(TType::AT);
				break;

			case '(':
				apptok(TType::LEFT_PAREN);
				break;
			case ')':
				apptok(TType::RIGHT_PAREN);
				break;
			case '{':
				apptok(TType::LEFT_BRACE);
				break;
			case '}':
				apptok(TType::RIGHT_BRACE);
				break;

			case '!':
				match('=') ? apptok(TType::BANG_EQ) : apptok(TType::BANG);
				break;
			case '=':
				match('=') ? apptok(TType::EQ_EQ) : apptok(TType::EQ);
				break;

			case '>':
				match('=') ? apptok(TType::GREATER_EQ) : apptok(TType::GREATER);
				break;
			case '<':
				match('=') ? apptok(TType::LESS_EQ) : apptok(TType::LESS);
				break;

			case '?':
				apptok(TType::QUE);
				break;
			case ':':
				apptok(TType::COL);
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

#undef VALID
#undef PEEK