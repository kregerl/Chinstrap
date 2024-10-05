#ifndef CHINSTRAP_BRACEPARSLET_H
#define CHINSTRAP_BRACEPARSLET_H

#include "PrefixParslet.h"

namespace Chinstrap {
    class BraceParslet : public PrefixParslet {
    public:
        explicit BraceParslet(Precedence precedence = Precedence::None);

        virtual std::shared_ptr<ASTNode> parse(Parser& parser, const Token& token);
    };
}

#endif //CHINSTRAP_BRACEPARSLET_H
