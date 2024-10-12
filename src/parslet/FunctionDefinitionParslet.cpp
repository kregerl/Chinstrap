#include "FunctionDefinitionParslet.h"
#include "../Parser.h"

namespace Chinstrap {
    FunctionDefinitionParslet::FunctionDefinitionParslet() : PrefixParslet(Precedence::None) {}

    std::shared_ptr<ASTNode> FunctionDefinitionParslet::parse(Parser &parser, const Token &token) {
        switch (token.type) {
            case TokenType::KW_def: {
                auto function_name = parser.consume(TokenType::Identifier);
                (void) parser.consume(TokenType::LParen);
                if (!parser.matches(TokenType::RParen)) {
                    std::vector<Token> parameters = {};
                    do {
                        if (parameters.size() >= 8)
                            throw std::runtime_error("Cannot have more than 8 parameters");

                        parameters.emplace_back(parser.consume(TokenType::Identifier));
                    } while (parser.matches(TokenType::Comma));
                    (void) parser.consume(TokenType::RParen);

                    std::shared_ptr<ASTNode> body = parser.parse_expression();
                    return std::make_shared<FunctionDefinitionNode>(function_name.value, parameters, body);
                }
            }
            default:
                break;
        }
    }
}

