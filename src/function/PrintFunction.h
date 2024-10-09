#ifndef CHINSTRAP_NATIVEFUNCTION_H
#define CHINSTRAP_NATIVEFUNCTION_H

#include "Function.h"

namespace Chinstrap {
    class PrintFunction : public Function {
    public:
        static Returnable call(std::vector<Returnable>& parameters);
    };
}

#endif