#include "InfixParslet.h"
#include "../Parser.h"

namespace Chinstrap {
    InfixParslet::InfixParslet(Precedence precedence) : Parslet(precedence) {}

    std::shared_ptr<ASTNode>
    InfixParslet::parse(Parser& parser, const Token& token, std::shared_ptr<ASTNode> lhs) {
        std::optional<BinaryOperationNode::Type> type = BinaryOperationNode::op_type_from_token_type(token.m_type);

        if (token.m_type == TokenType::Equals && !parser.matches(TokenType::Equals)) {
            auto identifier_node = std::dynamic_pointer_cast<IdentifierNode>(lhs);
            if (identifier_node != nullptr) {
                auto identifier = identifier_node->value();
                return std::make_shared<AssignmentNode>(identifier, parser.parse_expression(get_precedence()));
            }
        }

        if (token.m_type == TokenType::GreaterThan && parser.matches(TokenType::GreaterThan)) {
            type = BinaryOperationNode::Type::ShiftRight;
        }

        if (token.m_type == TokenType::LessThan && parser.matches(TokenType::LessThan)) {
            type = BinaryOperationNode::Type::ShiftLeft;
        }

        // Parse rhs and for binary node with 'lhs' 'token.m_type' 'rhs'
        auto rhs = parser.parse_expression(get_precedence());

        if (!type.has_value()) {
            throw std::runtime_error("Invalid token m_type for binary operation node");
        }
        return std::make_shared<BinaryOperationNode>(type.value(), std::move(lhs), rhs);
    }
}