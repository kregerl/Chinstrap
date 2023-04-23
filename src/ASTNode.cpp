#include <cstdlib>
#include "ASTNode.h"

namespace Chinstrap {
    IntegerNode::IntegerNode(uint64_t value) : m_value(value) {}

    int64_t IntegerNode::accept(Visitor& visitor) {
        return m_value;
    }

    BinaryOperationNode::BinaryOperationNode(
            Chinstrap::TokenType op_type,
            std::shared_ptr<ASTNode> left_child,
            std::shared_ptr<ASTNode> right_child
    ) : m_type(op_type),
        m_left_child(std::move(left_child)),
        m_right_child(std::move(right_child)) {}

    int64_t BinaryOperationNode::accept(Visitor& visitor) {
        switch (m_type) {
            case TokenType::Plus:return m_left_child->accept(visitor) + m_right_child->accept(visitor);
            case TokenType::Minus:return m_left_child->accept(visitor) - m_right_child->accept(visitor);
            case TokenType::Asterisk:return m_left_child->accept(visitor) * m_right_child->accept(visitor);
            case TokenType::Slash:return m_left_child->accept(visitor) / m_right_child->accept(visitor);
            default:
                // TODO: Throw error when token is not valid for a binary op node
                break;
        }
    }

    PrefixOperationNode::PrefixOperationNode(TokenType op_type, std::shared_ptr<ASTNode> child)
            : m_type(op_type), m_child(std::move(child)) {}

    int64_t PrefixOperationNode::accept(Visitor& visitor) {
        switch (m_type) {
            case TokenType::Minus: return -m_child->accept(visitor);
            default:
                // TODO: Throw error when token is not valid for a binary op node
                break;
        }
    }

    PostfixOperationNode::PostfixOperationNode(std::shared_ptr<ASTNode> child, TokenType op_type)
            : m_child(std::move(child)), m_type(op_type) {}

    int64_t PostfixOperationNode::accept(Visitor& visitor) {
        switch (m_type) {
//            case TokenType::Exclamation: return
        }
        return 0;
    }
}

