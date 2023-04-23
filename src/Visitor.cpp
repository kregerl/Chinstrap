#include "Visitor.h"
#include "ASTNode.h"

namespace Chinstrap {


    int64_t Interpreter::visit(Chinstrap::IntegerNode& node) {
        return node.accept(*this);
    }

    int64_t Interpreter::visit(BinaryOperationNode& node) {
        return node.accept(*this);
    }

    int64_t Interpreter::visit(PrefixOperationNode& node) {
        return node.accept(*this);
    }

    int64_t Interpreter::visit(PostfixOperationNode& node) {
        return node.accept(*this);
    }
}

