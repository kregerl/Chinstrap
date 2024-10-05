#include "BraceParslet.h"
#include "../Parser.h"

namespace Chinstrap {
    BraceParslet::BraceParslet(Precedence precedence) : PrefixParslet(precedence) {}

    std::shared_ptr<ASTNode> BraceParslet::parse(Parser& parser, const Token& token) {
        auto expressions = std::vector<std::shared_ptr<ASTNode>>();

        while (!parser.try_consume(TokenType::RBrace).has_value()) {
            expressions.push_back(parser.parse_expression());
        }
        return std::make_shared<BraceNode>(expressions);
    }
}
