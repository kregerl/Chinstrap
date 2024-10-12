#include "Function.h"

namespace Chinstrap {
    Function::Function(std::vector<Token> parameters, std::shared_ptr<ASTNode> body)
            : m_parameters(std::move(parameters)), m_body(std::move(body)) {}
}

