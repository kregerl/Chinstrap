#ifndef CHINSTRAP_EXCEPTION_H
#define CHINSTRAP_EXCEPTION_H

#include <exception>
#include <utility>

class EvaluatorException : public std::exception {
public:
    explicit EvaluatorException(std::string message) : m_msg(std::move(message)) {}

    ~EvaluatorException() noexcept override = default;

    [[nodiscard]] const char* what() const noexcept override {
        return m_msg.c_str();
    }

private:
    std::string m_msg{};
};

#endif //CHINSTRAP_EXCEPTION_H
