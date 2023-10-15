#include <iostream>
#include <cstdlib>
#include "ASTNode.h"

namespace Chinstrap {
    NumberNode::NumberNode(double value) : m_value(value) {}

    void NumberNode::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    ListNode::ListNode(std::vector<std::shared_ptr<ASTNode>> children) : m_children(std::move(children)){}

    void ListNode::accept(Visitor &visitor) {
        visitor.visit(*this);
    }

    BinaryOperationNode::BinaryOperationNode(
            BinaryOperationNode::Type op_type,
            std::shared_ptr<ASTNode> left_child,
            std::shared_ptr<ASTNode> right_child)
            : m_type(op_type),
              m_left_child(std::move(left_child)),
              m_right_child(std::move(right_child)) {}

    void BinaryOperationNode::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    PrefixOperationNode::PrefixOperationNode(Token token, std::shared_ptr<ASTNode> child)
            : m_token(std::move(token)), SingleChildNode(std::move(child)) {}

    void PrefixOperationNode::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    PostfixOperationNode::PostfixOperationNode(std::shared_ptr<ASTNode> child, Token token)
            : SingleChildNode(std::move(child)), m_token(std::move(token)) {}

    void PostfixOperationNode::accept(Visitor& visitor) {
        visitor.visit(*this);
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
}

