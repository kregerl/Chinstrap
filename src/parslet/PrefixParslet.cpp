#include "PrefixParslet.h"
#include "../Parser.h"

namespace Chinstrap {
    PrefixParslet::PrefixParslet(Precedence precedence) : Parslet(precedence) {}

    std::shared_ptr<ASTNode> PrefixParslet::parse(Parser& parser, const Token& token) {
        // If precedence is still none after parens, then its assumed its a number
        if (get_precedence() == Precedence::None) {
            switch (token.m_type) {
                case TokenType::Integer:
                    return std::make_shared<IntegerNode>(std::stoi(token.m_value));
                case TokenType::Real:
                    return std::make_shared<RealNode>(std::stod(token.m_value));
                case TokenType::Identifier: {
                    if (!parser.matches(TokenType::LParen)) {
                        return std::make_shared<IdentifierNode>(token.m_value);
                    }
                    auto parameters = std::vector<std::shared_ptr<ASTNode>>();
                    while (!parser.matches(TokenType::RParen)) {
                        parameters.emplace_back(parser.parse_expression());
                    }
                    return std::make_shared<FunctionNode>(token.m_value, parameters);
                }
                case TokenType::KW_return:{
                    auto expression = parser.parse_expression();
                    return std::make_shared<ReturnNode>(expression);
                }
                default: break;
            }
        }

        // Otherwise, parse as unary expression
        auto operand = parser.parse_expression(get_precedence());
        return std::make_shared<PrefixOperationNode>(token, operand);
    }
}
