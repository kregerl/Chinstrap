#ifndef CHINSTRAP_NATIVEFUNCTION_H
#define CHINSTRAP_NATIVEFUNCTION_H

#include "Function.h"

namespace Chinstrap {
    class PrintFunction : public Function {
    public:
        explicit PrintFunction(const std::unordered_map<std::string, TokenType> parameters);

        virtual Returnable call(const std::unordered_map<std::string, Returnable> parameters) override;
    };
}

#endif