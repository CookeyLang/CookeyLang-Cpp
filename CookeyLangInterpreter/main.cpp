#include "main.h"

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << "<CookeyLang> You must provide one input file!" << std::endl;
		return 1;
	}
	else
	{
		std::ifstream file(argv[1]);

		if (!file.is_open())
		{
			std::cout << "<CookeyLang> File does not exist!" << std::endl;
		}

		std::string line, code;

		while (std::getline(file, line))
		{
			code += line + '\n';
		}

		Lexer lexer(code, argv[1]);
		std::vector<Token> tokens = lexer.init();

		Parser parser(tokens, argv[1]);
		Expr* ast = parser.init();

		ast->visit();
	}

	return 0;
}