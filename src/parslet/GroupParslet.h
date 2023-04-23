#ifndef CHINSTRAP_GROUPPARSLET_H
#define CHINSTRAP_GROUPPARSLET_H

#include "PrefixParslet.h"

namespace Chinstrap {
    class GroupParslet : public PrefixParslet {
    public:
        GroupParslet();

        std::shared_ptr<ASTNode> parse(Parser& parser, const Token& token) override;
    };
}

#endif //CHINSTRAP_GROUPPARSLET_H
