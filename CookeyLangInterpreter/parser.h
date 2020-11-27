#ifndef PARSER_H
#define PARSER_H

#include "token.h"

#include "expr.h"

#include <vector>
#include <functional>

void parser(std::vector<Token> tokens, std::string file);

#endif