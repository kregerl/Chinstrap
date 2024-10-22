#include "InfixParslet.h"

#include <optional>

#include "../Parser.h"

namespace Chinstrap {
    InfixParslet::InfixParslet(Precedence precedence) : Parslet(precedence) {
    }

    std::shared_ptr<ASTNode>
    InfixParslet::parse(Parser &parser, const Token &token, std::shared_ptr<ASTNode> lhs) {
        std::optional<BinaryOperationNode::Type> type = BinaryOperationNode::op_type_from_token_type(token.m_type);

        if (token.m_type == TokenType::Equals && parser.matches(TokenType::Equals)) {
            type = BinaryOperationNode::Type::EqualTo;
            goto create_node;
        }

        if (token.m_type == TokenType::Exclamation) {
            if (parser.matches(TokenType::Equals)) {
                type = BinaryOperationNode::Type::NotEqualTo;
                goto create_node;
            }
            return std::make_shared<PostfixOperationNode>(lhs, token);
        }

        if (token.m_type == TokenType::GreaterThan) {
            if (parser.matches(TokenType::GreaterThan)) {
                type = BinaryOperationNode::Type::ShiftRight;
                goto create_node;
            }
            if (parser.matches(TokenType::Equals)) {
                type = BinaryOperationNode::Type::GreaterThanEqual;
                goto create_node;
            }
            type = BinaryOperationNode::Type::GreaterThan;
            goto create_node;
        }

        if (token.m_type == TokenType::LessThan) {
            if (parser.matches(TokenType::LessThan)) {
                type = BinaryOperationNode::Type::ShiftLeft;
                goto create_node;
            }
            if (parser.matches(TokenType::Equals)) {
                type = BinaryOperationNode::Type::LessThanEqual;
                goto create_node;
            }
            type = BinaryOperationNode::Type::LessThan;
            goto create_node;
        }

        if (!type.has_value()) {
            throw std::runtime_error("Invalid token type for binary operation node");
        }
    create_node:
        // Parse rhs and for binary node with 'lhs' 'token.m_type' 'rhs'
        auto rhs = parser.parse_expression(get_precedence());
        return std::make_shared<BinaryOperationNode>(type.value(), std::move(lhs), rhs);
    }
}
