#ifndef CHINSTRAP_STATEMENTPARSLET_H
#define CHINSTRAP_STATEMENTPARSLET_H

#include "Parslet.h"

namespace Chinstrap {
    class StatementParslet : public Parslet {
    public:
        explicit StatementParslet(Precedence precedence = Precedence::None);

        virtual std::shared_ptr<ASTNode> parse(Parser& parser, const Token& token);
    };
}

#endif //CHINSTRAP_STATEMENTPARSLET_H
