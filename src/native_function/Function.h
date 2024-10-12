#ifndef CHINSTRAP_FUNCTION_H
#define CHINSTRAP_FUNCTION_H

#include <memory>
#include <vector>
#include "../Lexer.h"
#include "../NodeForward.h"

namespace Chinstrap {
    class ASTNode;

    class Function {
    public:
        explicit Function(std::vector<Token> parameters, std::shared_ptr<ASTNode> body);

        [[nodiscard]] const std::vector<Token> &parameters() const { return m_parameters; }

        [[nodiscard]] std::shared_ptr<ASTNode> body() const { return m_body; }

    private:
        std::vector<Token> m_parameters;
        std::shared_ptr<ASTNode> m_body;
    };
}

#endif //CHINSTRAP_FUNCTION_H
