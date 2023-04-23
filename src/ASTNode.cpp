#include <iostream>
#include <cstdlib>
#include "ASTNode.h"

namespace Chinstrap {
    IntegerNode::IntegerNode(uint64_t value) : m_value(value) {}

    int64_t IntegerNode::accept(Visitor& visitor) {
        return m_value;
    }

    void IntegerNode::accept_print(Visitor& visitor, int* indent_amount) {
        std::cout << std::string(*indent_amount, ' ') << "IntegerNode: " << m_value << std::endl;
    }

    BinaryOperationNode::BinaryOperationNode(
            BinaryOperationNode::Type op_type,
            std::shared_ptr<ASTNode> left_child,
            std::shared_ptr<ASTNode> right_child)
            : m_type(op_type),
              m_left_child(std::move(left_child)),
              m_right_child(std::move(right_child)) {}

    int64_t BinaryOperationNode::accept(Visitor& visitor) {
        switch (m_type) {
            case Type::Addition: return m_left_child->accept(visitor) + m_right_child->accept(visitor);
            case Type::Subtraction: return m_left_child->accept(visitor) - m_right_child->accept(visitor);
            case Type::Multiplication: return m_left_child->accept(visitor) * m_right_child->accept(visitor);
            case Type::Division: return m_left_child->accept(visitor) / m_right_child->accept(visitor);
            case Type::ShiftLeft: return m_left_child->accept(visitor) << m_right_child->accept(visitor);
            case Type::ShiftRight: return m_left_child->accept(visitor) >> m_right_child->accept(visitor);
            default:
                // TODO: Throw error when token is not valid for a binary op node
                break;
        }
    }

    void BinaryOperationNode::accept_print(Visitor& visitor, int* indent_amount) {
        std::string op;
        switch (m_type) {
            case Type::Addition: {
                op = "+";
                break;
            }
            case Type::Subtraction: {
                op = "-";
                break;
            }
            case Type::Multiplication: {
                op = "*";
                break;
            }
            case Type::Division: {
                op = "/";
                break;
            }
            case Type::Modulus: {
                op = "%";
                break;
            }
            case Type::And: {
                op = "&";
                break;
            }
            case Type::Or: {
                op = "|";
                break;
            }
            case Type::ShiftLeft: {
                op = "<<";
                break;
            }
            case Type::ShiftRight: {
                op = ">>";
                break;
            }
        }

        std::cout << std::string(*indent_amount, ' ') << "BinaryOperationNode(" << op << "): " << std::endl;
        *indent_amount += 4;
        m_left_child->accept_print(visitor, indent_amount);
        m_right_child->accept_print(visitor, indent_amount);
        *indent_amount -= 4;
    }

    PrefixOperationNode::PrefixOperationNode(Token token, std::shared_ptr<ASTNode> child)
            : m_token(std::move(token)), m_child(std::move(child)) {}

    int64_t PrefixOperationNode::accept(Visitor& visitor) {
        switch (m_token.type) {
            case TokenType::Minus: return -m_child->accept(visitor);
            default:
                // TODO: Throw error when token is not valid for a binary op node
                break;
        }
    }

    void PrefixOperationNode::accept_print(Visitor& visitor, int* indent_amount) {
        std::cout << std::string(*indent_amount, ' ') << "PrefixOperationNode(" << m_token.value << "): " << std::endl;
        *indent_amount += 4;
        m_child->accept_print(visitor, indent_amount);
        *indent_amount -= 4;
    }

    PostfixOperationNode::PostfixOperationNode(std::shared_ptr<ASTNode> child, Token token)
            : m_child(std::move(child)), m_token(std::move(token)) {}

    int64_t PostfixOperationNode::accept(Visitor& visitor) {
        switch (m_token.type) {
            case TokenType::Exclamation: return factorial(m_child->accept(visitor));
            default:break;
        }
        return 0;
    }

    int64_t PostfixOperationNode::factorial(int64_t n) {
        int64_t factorial = 1;
        if (n < 0) {
            throw std::runtime_error("Cannot take the factorial of a negative number.");
        }
        for (int i = 1; i <= n; i++) {
            factorial *= i;
        }
        return factorial;
    }

    void PostfixOperationNode::accept_print(Visitor& visitor, int* indent_amount) {
        std::cout << std::string(*indent_amount, ' ') << "PostfixOperationNode(" << m_token.value << "): " << std::endl;
        *indent_amount += 4;
        m_child->accept_print(visitor, indent_amount);
        *indent_amount -= 4;
    }
}

