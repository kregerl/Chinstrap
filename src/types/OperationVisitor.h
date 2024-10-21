#ifndef CHINSTRAP_OPERATIONVISITOR_H
#define CHINSTRAP_OPERATIONVISITOR_H

#include <type_traits>
#include <stdexcept>
#include "../Returnable.h"

#define BINARY_FALLBACK_OPERATOR template<typename T, typename U> \
auto operator()(const T&, const U&) const -> Chinstrap::Returnable { \
    throw std::runtime_error("Operation not supported for these types.");\
}\

#define UNARY_FALLBACK_OPERATOR template<typename T> \
auto operator()(const T&) const -> Chinstrap::Returnable { \
    throw std::runtime_error("Operation not supported for these types.");\
}\

namespace Chinstrap {

    template<typename T, typename U>
    struct Promote {
        using Type = decltype(T() + U());
    };

    template<typename Operation>
    struct OperationVisitor {
        Operation m_op;

        explicit OperationVisitor(Operation op) : m_op(std::move(op)) {}
    };

    template<typename Operation>
    struct NumericBinaryOperationVisitor : public OperationVisitor<Operation> {
        explicit NumericBinaryOperationVisitor(Operation op) : OperationVisitor<Operation>(std::move(op)) {}

        template<typename T, typename U>
        auto operator()(T &a, U &b) const
        -> std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<U>, Chinstrap::Returnable> {
            using ResultType = typename Promote<T, U>::Type;
            return this->m_op(static_cast<ResultType>(a), static_cast<ResultType>(b));
        }

        BINARY_FALLBACK_OPERATOR
    };

    template<typename Operation>
    struct NumericUnaryOperationVisitor : public OperationVisitor<Operation> {
        explicit NumericUnaryOperationVisitor(Operation op) : OperationVisitor<Operation>(std::move(op)) {}

        template<typename T>
        auto operator()(T &a) const
        -> std::enable_if_t<std::is_arithmetic_v<T>, Chinstrap::Returnable> {
            return this->m_op(a);
        }

        UNARY_FALLBACK_OPERATOR
    };

    template<typename Operation>
    struct IntegerBinaryOperationVisitor : public OperationVisitor<Operation> {
        explicit IntegerBinaryOperationVisitor(Operation op) : OperationVisitor<Operation>(std::move(op)) {}

        template<typename T, typename U>
        auto operator()(T &a, U &b) const
        -> std::enable_if_t<
                std::numeric_limits<T>::is_integer && std::numeric_limits<U>::is_integer, Chinstrap::Returnable> {
            using ResultType = typename Promote<T, U>::Type;
            return this->m_op(static_cast<ResultType>(a), static_cast<ResultType>(b));
        }

        BINARY_FALLBACK_OPERATOR
    };

    template<typename Operation>
    struct IntegerUnaryOperationVisitor : public OperationVisitor<Operation> {
        explicit IntegerUnaryOperationVisitor(Operation op) : OperationVisitor<Operation>(std::move(op)) {}

        template<typename T>
        auto operator()(T &a) const
        -> std::enable_if_t<std::numeric_limits<T>::is_integer, Chinstrap::Returnable> {
            return this->m_op(a);
        }

        UNARY_FALLBACK_OPERATOR
    };

    struct DivisionVisitor {
        template<typename T, typename U>
        auto operator()(T &a, U &b) const
        -> std::enable_if_t<std::is_arithmetic_v<T> && std::is_arithmetic_v<U>, Chinstrap::Returnable> {
            using ResultType = typename Promote<T, U>::Type;
            if (b == 0)
                throw EvaluatorException("Division by 0");
            return static_cast<ResultType>(a) / static_cast<ResultType>(b);
        }

        BINARY_FALLBACK_OPERATOR
    };

    namespace Operation {
        template<typename T>
        static auto
        factorial(T literal) -> std::enable_if_t<std::numeric_limits<T>::is_integer, Chinstrap::Returnable> {
            T n = literal;
            T factorial = 1;
            if (n < 0) {
                throw std::runtime_error("Cannot take the factorial of a negative number.");
            }
            for (int i = 1; i <= n; i++) {
                factorial *= i;
            }
            return factorial;
        }
    };
}

#endif //CHINSTRAP_OPERATIONVISITOR_H
