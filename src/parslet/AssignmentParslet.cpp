#include "AssignmentParslet.h"

#include "../Parser.h"

namespace Chinstrap {
    AssignmentParslet::AssignmentParslet(Precedence precedence) : PrefixParslet(precedence) {
    }

    std::shared_ptr<ASTNode> AssignmentParslet::parse(Parser &parser, const Token &token) {
        auto identifier = parser.consume(TokenType::Identifier);
        (void) parser.consume(TokenType::Equals);
        auto expression = parser.parse_expression();
        (void) parser.consume(TokenType::Semicolon);
        return std::make_shared<AssignmentNode>(identifier.m_value, expression);
    }
}
