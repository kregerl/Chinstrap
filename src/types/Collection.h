#ifndef CHINSTRAP_COLLECTION_H
#define CHINSTRAP_COLLECTION_H

#include <variant>
#include <vector>
#include "Literal.h"
#include "../Visit.h"

namespace Chinstrap {

    class Collection;

    using Returnable = std::variant<std::monostate, int64_t, double, bool, Collection>;

    class Collection : public std::vector<Returnable> {
        friend std::ostream &operator<<(std::ostream &os, const Collection &c) {
            os << '[';
            for (int i = 0; i < c.size(); i++) {
                auto element = c.at(i);
                std::visit(overloaded{
                        [&os](int64_t &literal) { os << std::to_string(literal); },
                        [&os](double &literal) { os << std::to_string(literal); },
                        [&os](Collection &collection) { os << collection; },
                        [&os](auto &a) { os << std::string("Unknown m_type!"); }
                }, element);
                if (i < c.size() - 1) {
                    os << std::string(", ");
                }
            }
            os << ']';
            return os;
        }
    };
}

#endif //CHINSTRAP_COLLECTION_H
