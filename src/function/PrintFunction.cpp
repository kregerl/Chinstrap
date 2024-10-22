#include "PrintFunction.h"
#include <iostream>
#include "../util/StringFormat.h"

namespace Chinstrap {
    Returnable PrintFunction::call(std::vector<Returnable> &parameters) {
        if (parameters.empty()) {
            throw std::runtime_error("PrintFunction::call: Expected 1 argument, got 0");
        }
        if (parameters.size() > 1) {
            throw std::runtime_error(string_format("PrintFunction::call: Expected 1 argument, got %ld",
                                                   parameters.size()));
        }

        auto &value = parameters[0];
        const auto string = std::visit(overloaded{
                [](int64_t &n) -> std::string { return std::to_string(n); },
                [](double &n) -> std::string { return std::to_string(n); },
                [](auto &n) -> std::string { return "<Unimplemented>"; }
        }, value);

        std::cout << string << std::endl;
        return {};
    }
}
