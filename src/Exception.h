#ifndef CHINSTRAP_EXCEPTION_H
#define CHINSTRAP_EXCEPTION_H

#include <exception>
#include <utility>
#include "Returnable.h"

class EvaluatorException : public std::exception {
public:
    explicit EvaluatorException(std::string message) : m_msg(std::move(message)) {}

    ~EvaluatorException() noexcept override = default;

    [[nodiscard]] const char *what() const noexcept override {
        return m_msg.c_str();
    }

private:
    std::string m_msg{};
};

namespace Chinstrap {
    class ReturnValueException : public std::exception {
    public:
        explicit ReturnValueException(Returnable value) : m_value(std::move(value)) {}

        ~ReturnValueException() noexcept override = default;

        [[nodiscard]] Returnable value() const { return m_value; }

    private:
        Returnable m_value;
    };
}

#endif //CHINSTRAP_EXCEPTION_H
