#ifndef CHINSTRAP_LISTPARSLET_H
#define CHINSTRAP_LISTPARSLET_H

#include "PrefixParslet.h"

namespace Chinstrap {

    class ListParslet : public PrefixParslet {
    public:
        ListParslet();

        ~ListParslet() = default;

        std::shared_ptr<ASTNode> parse(Parser& parser, const Token& token) override;
    };
}


#endif //CHINSTRAP_LISTPARSLET_H
