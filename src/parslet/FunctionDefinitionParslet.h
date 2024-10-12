#ifndef CHINSTRAP_FUNCTIONDEFINITIONPARSLET_H
#define CHINSTRAP_FUNCTIONDEFINITIONPARSLET_H

#include "PrefixParslet.h"

namespace Chinstrap {
    class FunctionDefinitionParslet : public PrefixParslet{
    public:
        FunctionDefinitionParslet();

        ~FunctionDefinitionParslet() = default;

        std::shared_ptr<ASTNode> parse(Parser& parser, const Token& token) override;
    };
}

#endif //CHINSTRAP_FUNCTIONDEFINITIONPARSLET_H
