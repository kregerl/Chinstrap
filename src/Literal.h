#ifndef CHINSTRAP_LITERAL_H
#define CHINSTRAP_LITERAL_H

#include <utility>
#include <string>

#define NUMERIC_OPERATOR_OVERLOAD(literal, op) literal operator op(const literal &n) { return literal(m_value op n.m_value); }

namespace Chinstrap {
    template<typename T>
    class Literal {
    public:
        explicit Literal(T value) : m_value(std::move(value)) {}

        [[nodiscard]] T get_value() const { return m_value; }

    protected:
        T m_value;
    };

    class NumericLiteral : public Literal<double> {
    public:
        explicit NumericLiteral(double value) : Literal<double>(value) {}

        friend bool operator==(const NumericLiteral &lhs, const NumericLiteral &rhs) {
            return lhs.m_value == rhs.m_value;
        }

        NUMERIC_OPERATOR_OVERLOAD(NumericLiteral, +);
        NUMERIC_OPERATOR_OVERLOAD(NumericLiteral, -);
        NUMERIC_OPERATOR_OVERLOAD(NumericLiteral, *);
        NUMERIC_OPERATOR_OVERLOAD(NumericLiteral, /);
    };

    class StringLiteral : public Literal<std::string> {
    public:
        explicit StringLiteral(std::string value) : Literal<std::string>(std::move(value)) {}

        friend bool operator==(const StringLiteral &lhs, const StringLiteral &rhs) {
            return lhs.m_value == rhs.m_value;
        }
    };
}


#endif //CHINSTRAP_LITERAL_H
