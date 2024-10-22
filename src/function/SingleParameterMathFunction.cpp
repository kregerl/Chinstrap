#include "SingleParameterMathFunction.h"
#include "../util/StringFormat.h"
#include "../types/OperationVisitor.h"

namespace Chinstrap {
    SingleParameterMathFunction::SingleParameterMathFunction(const std::function<double(double)> &function)
            : m_function(function) {}

    Returnable SingleParameterMathFunction::call(std::vector<Returnable> &parameters) {
        if (parameters.empty()) {
            throw std::runtime_error("SingleParameterMathFunction::call: Expected 1 argument, got 0");
        }
        if (parameters.size() > 1) {
            throw std::runtime_error(string_format("SingleParameterMathFunction::call: Expected 1 argument, got %ld",
                                                   parameters.size()));
        }

        auto &value = parameters[0];
        const auto real_value = std::visit(NumericUnaryOperationVisitor{
                [this](auto n) { return m_function(n); }}, value);

        return real_value;
    };

}