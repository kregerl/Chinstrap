#ifndef CHINSTRAP_PREFIXPARSLET_H
#define CHINSTRAP_PREFIXPARSLET_H

#include "Parslet.h"

namespace Chinstrap {
    class PrefixParslet : public Parslet {
    public:
        explicit PrefixParslet(Precedence precedence = Precedence::None);

        virtual std::shared_ptr<ASTNode> parse(Parser& parser, const Token& token);
    };
}

#endif //CHINSTRAP_PREFIXPARSLET_H
