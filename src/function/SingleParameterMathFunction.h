#ifndef CHINSTRAP_SINGLEPARAMETERMATHFUNCTION_H
#define CHINSTRAP_SINGLEPARAMETERMATHFUNCTION_H

#include <functional>
#include "NativeFunction.h"

namespace Chinstrap {
    class SingleParameterMathFunction : public NativeFunction {
    public:
        explicit SingleParameterMathFunction(const std::function<double(double)> &function);

        Returnable call(std::vector<Returnable>& parameters) override;

    private:
        std::function<double(double)> m_function;
    };
}


#endif //CHINSTRAP_SINGLEPARAMETERMATHFUNCTION_H
