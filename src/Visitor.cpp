#include <iostream>
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

    int64_t PrettyPrinter::visit(IntegerNode& node) {
        node.accept_print(*this, &m_indent_amount);
        return 0;
    }

    int64_t PrettyPrinter::visit(BinaryOperationNode& node) {
        node.accept_print(*this, &m_indent_amount);
        return 0;
    }

    int64_t PrettyPrinter::visit(PrefixOperationNode& node) {
        node.accept_print(*this, &m_indent_amount);
        return 0;
    }

    int64_t PrettyPrinter::visit(PostfixOperationNode& node) {
        node.accept_print(*this, &m_indent_amount);
        return 0;
    }
}

