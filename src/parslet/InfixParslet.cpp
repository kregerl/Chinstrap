#include "InfixParslet.h"
#include "../Parser.h"

namespace Chinstrap {
    InfixParslet::InfixParslet(Precedence precedence) : Parslet(precedence) {}

    std::shared_ptr<ASTNode>
    InfixParslet::parse(Parser& parser, const Token& token, std::shared_ptr<ASTNode> lhs) {
        // Parse rhs and for binary node with 'lhs' 'token.type' 'rhs'
        auto rhs = parser.parse_expression();
        return std::make_shared<BinaryOperationNode>(token.type, std::move(lhs), rhs);
    }
}