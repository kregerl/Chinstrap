#include "PostfixParslet.h"

namespace Chinstrap {
    PostfixParslet::PostfixParslet(Precedence precedence) : InfixParslet(precedence) {}

    std::shared_ptr<ASTNode> PostfixParslet::parse(Parser& parser, const Token& token, std::shared_ptr<ASTNode> lhs) {
        return std::make_shared<PostfixOperationNode>(lhs, token);
    }

}

