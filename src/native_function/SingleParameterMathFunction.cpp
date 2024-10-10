#include "SingleParameterMathFunction.h"
#include "../util/StringFormat.h"
#include "../Exception.h"

namespace Chinstrap {
    SingleParameterMathFunction::SingleParameterMathFunction(const std::function<double(double)> &function) : m_function(function) {}

    Returnable SingleParameterMathFunction::call(std::vector<Returnable> &parameters) {
        if (parameters.empty()) {
            throw std::runtime_error("SineFunction::call: Expected 1 argument, got 0");
        }
        if (parameters.size() > 1) {
            throw std::runtime_error(string_format("SineFunction::call: Expected 1 argument, got %ld",
                                                   parameters.size()));
        }

        auto &value = parameters[0];
        const auto real_value = std::visit(overloaded{
                [this](IntegerLiteral &n) -> Returnable { return RealLiteral(m_function(n.get_value())); },
                [this](RealLiteral &n) -> Returnable { return RealLiteral(m_function(n.get_value())); },
                [](auto &n) -> Returnable { throw EvaluatorException("Error"); }
        }, value);

        return real_value;
    };

}