#ifndef CHINSTRAP_LITERAL_H
#define CHINSTRAP_LITERAL_H

#include <utility>
#include <string>
#include <sstream>

#define SINGLE_NUMERIC_OPERATOR_OVERLOAD_TEMPLATE(literal, op) friend literal operator op(const literal &a, const literal &b);
#define SINGLE_NUMERIC_OPERATOR_OVERLOAD_IMPL(literal, op) literal operator op(const literal &a, const literal &b) { return literal(a.get_value() op b.get_value()); }

#define NUMERIC_OPERATOR_OVERLOAD_TEMPLATE(literal1, literal2, returntype, op) friend returntype operator op(const literal1 &a, const literal2 &b);
#define NUMERIC_OPERATOR_OVERLOAD_IMPL(literal1, literal2, returntype, op) returntype operator op(const literal1 &a, const literal2 &b) { return returntype(a.get_value() op b.get_value()); }


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

        explicit IntegerLiteral(const int64_t value) : Literal(value) {}

        void invert() { m_value = -m_value; }

        friend bool operator==(const IntegerLiteral &lhs, const IntegerLiteral &rhs) {
            return lhs.m_value == rhs.m_value;
        }

        SINGLE_NUMERIC_OPERATOR_OVERLOAD_TEMPLATE(IntegerLiteral, +);

        NUMERIC_OPERATOR_OVERLOAD_TEMPLATE(IntegerLiteral, RealLiteral, RealLiteral, +);

        SINGLE_NUMERIC_OPERATOR_OVERLOAD_TEMPLATE(IntegerLiteral, -);

        SINGLE_NUMERIC_OPERATOR_OVERLOAD_TEMPLATE(IntegerLiteral, *);

        SINGLE_NUMERIC_OPERATOR_OVERLOAD_TEMPLATE(IntegerLiteral, /);

        NUMERIC_OPERATOR_OVERLOAD_TEMPLATE(IntegerLiteral, RealLiteral, RealLiteral, /);

        SINGLE_NUMERIC_OPERATOR_OVERLOAD_TEMPLATE(IntegerLiteral, %);

        SINGLE_NUMERIC_OPERATOR_OVERLOAD_TEMPLATE(IntegerLiteral, &);

        SINGLE_NUMERIC_OPERATOR_OVERLOAD_TEMPLATE(IntegerLiteral, |);

        SINGLE_NUMERIC_OPERATOR_OVERLOAD_TEMPLATE(IntegerLiteral, <<);

        SINGLE_NUMERIC_OPERATOR_OVERLOAD_TEMPLATE(IntegerLiteral, >>);

        friend std::ostream &operator<<(std::ostream &os, const IntegerLiteral &n) {
            os << n.m_value;
            return os;
        }
    };


    class RealLiteral : public Literal<double> {
    public:
        RealLiteral() : Literal(0.0f) {}

        explicit RealLiteral(const double value) : Literal(value) {}

        void invert() { m_value = -m_value; }

        friend bool operator==(const RealLiteral &lhs, const RealLiteral &rhs) {
            return lhs.m_value == rhs.m_value;
        }

        SINGLE_NUMERIC_OPERATOR_OVERLOAD_TEMPLATE(RealLiteral, +);

        SINGLE_NUMERIC_OPERATOR_OVERLOAD_TEMPLATE(RealLiteral, -);

        SINGLE_NUMERIC_OPERATOR_OVERLOAD_TEMPLATE(RealLiteral, *);

        SINGLE_NUMERIC_OPERATOR_OVERLOAD_TEMPLATE(RealLiteral, /);

        NUMERIC_OPERATOR_OVERLOAD_TEMPLATE(RealLiteral, IntegerLiteral, RealLiteral, /);


        friend std::ostream &operator<<(std::ostream &os, const RealLiteral &n) {
            os << n.m_value;
            return os;
        }
    };

    class StringLiteral : public Literal<std::string> {
    public:
        explicit StringLiteral(std::string value) : Literal(std::move(value)) {}

        friend bool operator==(const StringLiteral &lhs, const StringLiteral &rhs) {
            return lhs.m_value == rhs.m_value;
        }
    };
}


#endif //CHINSTRAP_LITERAL_H
