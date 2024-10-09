#ifndef CHINSTRAP_PRINTFUNCTION_H
#define CHINSTRAP_PRINTFUNCTION_H

#include "NativeFunction.h"

namespace Chinstrap {
    class PrintFunction : public NativeFunction {
    public:
        Returnable call(std::vector<Returnable>& parameters) override;
    };
}

#endif