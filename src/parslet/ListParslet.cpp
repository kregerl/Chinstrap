#include <vector>
#include "ListParslet.h"
#include "../Parser.h"

namespace Chinstrap {
    ListParslet::ListParslet() : PrefixParslet(Precedence::None) {}

    std::shared_ptr<ASTNode> ListParslet::parse(Parser &parser, const Token &token) {
        std::vector<std::shared_ptr<ASTNode>> children;
        children.emplace_back(parser.parse_expression());
        while (parser.try_consume(TokenType::Comma).has_value()) {
            children.emplace_back(parser.parse_expression());
        }
        parser.consume(TokenType::RBracket);
        return std::make_shared<ListNode>(children);
    }
}



