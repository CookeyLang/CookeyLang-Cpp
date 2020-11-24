#include "main.h"

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << "[ 0 : 0 ] You must provide one input file!" << std::endl;
		return 1;
	}
	else
	{
		std::ifstream file(argv[1]);
		std::string line, code;

		while (std::getline(file, line))
		{
			code += line + '\n';
		}
		
		std::vector<Token> tokens = lexer(code);
	}

	return 0;
}