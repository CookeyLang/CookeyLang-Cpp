#ifndef LEXER_H
#define LEXER_H

#include "lexer.h"
#include "token.h"

#include <vector>
#include <functional>
#include <map>

std::vector<Token> lexer(std::string code);

#endif