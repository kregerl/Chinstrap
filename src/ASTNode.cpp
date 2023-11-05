#include <iostream>
#include <cstdlib>
#include <utility>
#include "ASTNode.h"

namespace Chinstrap {
    IntegerNode::IntegerNode(int64_t value) : ValueNode(value) {}

    RealNode::RealNode(double value) : ValueNode(value) {}

    IdentifierNode::IdentifierNode(std::string value) : ValueNode(std::move(value)) {}

    ListNode::ListNode(std::vector<std::shared_ptr<ASTNode>> children) : m_children(std::move(children)) {}

    BinaryOperationNode::BinaryOperationNode(
            BinaryOperationNode::Type op_type,
            std::shared_ptr<ASTNode> left_child,
            std::shared_ptr<ASTNode> right_child)
            : m_type(op_type),
              m_left_child(std::move(left_child)),
              m_right_child(std::move(right_child)) {}


    PrefixOperationNode::PrefixOperationNode(Token token, std::shared_ptr<ASTNode> child)
            : m_token(std::move(token)), SingleChildNode(std::move(child)) {}

    PostfixOperationNode::PostfixOperationNode(std::shared_ptr<ASTNode> child, Token token)
            : SingleChildNode(std::move(child)), m_token(std::move(token)) {}

    IntegerLiteral PostfixOperationNode::factorial(IntegerLiteral literal) {
        int64_t n = literal.get_value();
        int64_t factorial = 1;
        if (n < 0) {
            throw std::runtime_error("Cannot take the factorial of a negative number.");
        }
        for (int i = 1; i <= n; i++) {
            factorial *= i;
        }
        return IntegerLiteral(factorial);
    }

    AssignmentNode::AssignmentNode(std::string identifier, std::shared_ptr<ASTNode> rhs) :
            m_rhs(std::move(rhs)), m_identifier(std::move(identifier)) {}
}

