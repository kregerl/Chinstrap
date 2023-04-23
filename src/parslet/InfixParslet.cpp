#include "InfixParslet.h"
#include "../Parser.h"

namespace Chinstrap {
    InfixParslet::InfixParslet(Precedence precedence) : Parslet(precedence) {}

    std::shared_ptr<ASTNode>
    InfixParslet::parse(Parser& parser, const Token& token, std::shared_ptr<ASTNode> lhs) {
        std::optional<BinaryOperationNode::Type> type = BinaryOperationNode::op_type_from_token_type(token.type);

        if (token.type == TokenType::GreaterThan && parser.matches(TokenType::GreaterThan)) {
            type = BinaryOperationNode::Type::ShiftRight;
        }

        if (token.type == TokenType::LessThan && parser.matches(TokenType::LessThan)) {
            type = BinaryOperationNode::Type::ShiftLeft;
        }

        // Parse rhs and for binary node with 'lhs' 'token.type' 'rhs'
        auto rhs = parser.parse_expression();

        if (!type.has_value()) {
            throw std::runtime_error("Invalid token type for binary operation node");
        }
        return std::make_shared<BinaryOperationNode>(type.value(), std::move(lhs), rhs);
    }
}