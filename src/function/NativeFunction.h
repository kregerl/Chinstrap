#ifndef CHINSTRAP_NATIVEFUNCTION_H
#define CHINSTRAP_NATIVEFUNCTION_H

#include <string>
#include <unordered_map>
#include "../Lexer.h"
#include "../Returnable.h"

namespace Chinstrap {
    class NativeFunction {
    public:
        virtual ~NativeFunction() = default;

        virtual Returnable call(std::vector<Returnable>& parameters) = 0;
    };
}

#endif