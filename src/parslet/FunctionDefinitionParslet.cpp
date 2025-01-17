#include "FunctionDefinitionParslet.h"
#include "../Parser.h"
#include "../util/StringFormat.h"

static constexpr uint32_t MAX_PARAMETERS = 255;
namespace Chinstrap {

    FunctionDefinitionParslet::FunctionDefinitionParslet() : PrefixParslet(Precedence::None) {}

    std::shared_ptr<ASTNode> FunctionDefinitionParslet::parse(Parser &parser, const Token &token) {
        auto function_name = parser.consume(TokenType::Identifier);
        (void) parser.consume(TokenType::LParen);
        std::vector<Token> parameters = {};
        if (!parser.matches(TokenType::RParen)) {
            // Non-empty function parameters
            do {
                if (parameters.size() >= MAX_PARAMETERS)
                    throw std::runtime_error(
                            string_format("Cannot have more than %d parameters", MAX_PARAMETERS));

                parameters.emplace_back(parser.consume(TokenType::Identifier));
            } while (parser.matches(TokenType::Comma));
            (void) parser.consume(TokenType::RParen);
        }

        std::shared_ptr<ASTNode> body = parser.parse_expression();
        if (body->id() == ASTNode::TypeId::Brace)
            std::static_pointer_cast<BraceNode>(body)->set_push_scope(false);
        return std::make_shared<FunctionDefinitionNode>(function_name.m_value, parameters, body);
    }
}

