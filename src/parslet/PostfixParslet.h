#ifndef CHINSTRAP_POSTFIXPARSLET_H
#define CHINSTRAP_POSTFIXPARSLET_H

#include "InfixParslet.h"

namespace Chinstrap {
    class PostfixParslet : public InfixParslet {
    public:
        explicit PostfixParslet(Precedence precedence);

        std::shared_ptr<ASTNode> parse(Parser& parser, const Token& token, std::shared_ptr<ASTNode> lhs) override;
    };
}


#endif //CHINSTRAP_POSTFIXPARSLET_H
