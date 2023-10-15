#ifndef CHINSTRAP_LITERAL_H
#define CHINSTRAP_LITERAL_H

#include <utility>
#include <string>
#include <sstream>

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

    class RealLiteral;

    class IntegerLiteral : public Literal<int64_t> {
    public:
        IntegerLiteral() : Literal(0) {}

        explicit IntegerLiteral(int64_t value) : Literal<int64_t>(value) {}

        friend bool operator==(const IntegerLiteral &lhs, const IntegerLiteral &rhs) {
            return lhs.m_value == rhs.m_value;
        }

        NUMERIC_OPERATOR_OVERLOAD(IntegerLiteral, +);

        NUMERIC_OPERATOR_OVERLOAD(IntegerLiteral, -);

        NUMERIC_OPERATOR_OVERLOAD(IntegerLiteral, *);

        NUMERIC_OPERATOR_OVERLOAD(IntegerLiteral, /);

        NUMERIC_OPERATOR_OVERLOAD(IntegerLiteral, %);

        NUMERIC_OPERATOR_OVERLOAD(IntegerLiteral, &);

        NUMERIC_OPERATOR_OVERLOAD(IntegerLiteral, |);

        NUMERIC_OPERATOR_OVERLOAD(IntegerLiteral, <<);

        NUMERIC_OPERATOR_OVERLOAD(IntegerLiteral, >>);

        friend std::ostream &operator<<(std::ostream &os, const IntegerLiteral &n) {
            os << n.m_value;
            return os;
        }
    };


    class RealLiteral : public Literal<double> {
    public:
        RealLiteral() : Literal(0.0f) {}

        explicit RealLiteral(double value) : Literal<double>(value) {}

        friend bool operator==(const RealLiteral &lhs, const RealLiteral &rhs) {
            return lhs.m_value == rhs.m_value;
        }

        NUMERIC_OPERATOR_OVERLOAD(RealLiteral, +);

        NUMERIC_OPERATOR_OVERLOAD(RealLiteral, -);

        NUMERIC_OPERATOR_OVERLOAD(RealLiteral, *);

        NUMERIC_OPERATOR_OVERLOAD(RealLiteral, /);

        friend std::ostream &operator<<(std::ostream &os, const RealLiteral &n) {
            os << n.m_value;
            return os;
        }
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
