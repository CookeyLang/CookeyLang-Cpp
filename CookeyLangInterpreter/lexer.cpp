#include "lexer.h"

// macros
#define VALID i < code.length()

// functions
static void bappend(std::vector<Token>* output, int line, int col, TType type, std::string val);

static void bnewline(int* line, int* col, int* i);
static void bnext(int* col, int* i);

static bool bmatch(std::string code, int* i, char c);

static bool isNum(char c);
static bool isAlpha(char c);
static bool isAlphaNum(char c);


std::vector<Token> lexer(std::string code)
{
	int line = 1, col = 1, i = 0;
	std::vector<Token> output;

	using namespace std::placeholders; // _1, _2, etc
	auto append = std::bind(bappend, &output, line, col, _1, _2);
	auto apptok = std::bind(bappend, &output, line, col, _1, "");
	auto newline = std::bind(bnewline, &line, &col, &i);
	auto next = std::bind(bnext, &col, &i);
	auto match = std::bind(bmatch, code, &i, _1);

	while (VALID) {
		char curr = code[i];

		switch (curr) {
		case '+': match('=') ? apptok(TType::PLUS_EQ) : match('+') ? apptok(TType::PLUS_PLUS) : apptok(TType::PLUS); break;
		case '-': match('=') ? apptok(TType::MINUS_EQ) : match('-') ? apptok(TType::MINUS_MINUS) : apptok(TType::MINUS); break;
		case '*': match('=') ? apptok(TType::TIMES_EQ) : apptok(TType::TIMES); break;
		case '/': match('=') ? apptok(TType::DIVIDE_EQ) : apptok(TType::DIVIDE); break;
		case '^': match('=') ? apptok(TType::POWER_EQ) : apptok(TType::POWER); break;

		case '%':
			if (match('%')) while (VALID && code[(int64_t)i + 1] != '\n') i++;
			else if (match('*')) {
				while (VALID && !(code[(int64_t)i + 1] == '*' && code[(int64_t)i + 2] == '%')) code[i] == '\n' ? newline() : next();

				if (code[(int64_t)i + 1] != '*' && code[(int64_t)i + 2] != '%') error(line, col, "Unterminated multi-line comment.");

				next(); // the *
				next(); // the %
			}
			else if (match('=')) apptok(TType::MODULO_EQ);
			else apptok(TType::MODULO);
			break;

		case ';': apptok(TType::SEMI); break;
		case ',': apptok(TType::COMMA); break;
		case '.': apptok(TType::DOT); break;
		case '@': apptok(TType::AT); break;

		case '(': apptok(TType::LEFT_PAREN); break;
		case ')': apptok(TType::RIGHT_PAREN); break;
		case '{': apptok(TType::LEFT_BRACE); break;
		case '}': apptok(TType::RIGHT_BRACE); break;

		case '!': match('=') ? apptok(TType::BANG_EQ) : apptok(TType::BANG); break;
		case '=': match('=') ? apptok(TType::EQ_EQ) : apptok(TType::EQ); break;

		case '>': match('=') ? apptok(TType::GREATER_EQ) : apptok(TType::GREATER); break;
		case '<': match('=') ? apptok(TType::LESS_EQ) : apptok(TType::LESS); break;

		case ' ':
		case '\r':
		case '\t':
			// ignore whitespace
			break;

		case '\n':
			newline();
			break;

		default:
			error(line, col, "Unexpected character %c", curr);
			break;
		}

		next();
	}

	return output;
};


static void bappend(std::vector<Token>* output, int line, int col, TType type, std::string val)
{
	output->push_back(Token(line, col, type, val));
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
	if (code[(int64_t)(*i) + 1] == c) {
		(*i)++;
		return true;
	}
	return false;
}

#undef VALID