#ifndef CHINSTRAP_FUNCTION_H
#define CHINSTRAP_FUNCTION_H

#include <string>
#include <unordered_map>
#include "../Lexer.h"
#include "../Returnable.h"

namespace Chinstrap {
    class Function {
    public:
        Function(const std::unordered_map<std::string, TokenType> parameters);

        virtual Returnable call(const std::unordered_map<std::string, Returnable> parameters) = 0;
    };
}

#endif