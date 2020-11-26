#ifndef PARSER_H
#define PARSER_H

#include "token.h"

#include <vector>
#include <functional>

void parser(std::vector<Token> tokens, std::string file);

#endif