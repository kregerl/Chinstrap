#ifndef ASSIGNMENTPARSLET_H
#define ASSIGNMENTPARSLET_H
#include "PrefixParslet.h"

namespace Chinstrap {
    class AssignmentParslet : public PrefixParslet {
    public:
        explicit AssignmentParslet(Precedence precedence = Precedence::Assignment);

        std::shared_ptr<ASTNode> parse(Parser& parser, const Token& token) override;
    };
}

#endif //ASSIGNMENTPARSLET_H
