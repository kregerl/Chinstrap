#ifndef CHINSTRAP_INFIXPARSLET_H
#define CHINSTRAP_INFIXPARSLET_H

#include "Parslet.h"

namespace Chinstrap {
    class InfixParslet : public Parslet {
    public:
        explicit InfixParslet(Precedence precedence);

        virtual std::shared_ptr<ASTNode> parse(Parser& parser, const Token& token, std::shared_ptr<ASTNode> lhs);
    };
}


#endif //CHINSTRAP_INFIXPARSLET_H
