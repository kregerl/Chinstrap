#include "PrefixParslet.h"
#include "../Parser.h"

namespace Chinstrap {
    PrefixParslet::PrefixParslet(Precedence precedence) : Parslet(precedence) {}

    std::shared_ptr<ASTNode> PrefixParslet::parse(Parser& parser, const Token& token) {
        // If precedence is still none after parens, then its assumed its a number
        if (get_precedence() == Precedence::None) {
            return std::make_shared<IntegerNode>(std::stoi(token.value));
        }

        // Otherwise, parse as unary expression
        auto operand = parser.parse_expression(get_precedence());
        return std::make_shared<PrefixOperationNode>(token, operand);
    }
}
