#include "GroupParslet.h"
#include "../Parser.h"

namespace Chinstrap {
    GroupParslet::GroupParslet() : PrefixParslet(Precedence::None) {}

    std::shared_ptr<ASTNode> GroupParslet::parse(Parser& parser, const Token& token) {
        auto expression = parser.parse_expression();
        parser.consume(TokenType::RParen);
        return expression;
    }
}