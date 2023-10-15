#include <iostream>
#include "Visitor.h"
#include "ASTNode.h"
#include "Exception.h"

#define RESULT_CASE(x) {result(x); break;}

namespace Chinstrap {

    template<typename Visitor, typename Visitable, typename ResultType>
    ResultType ValueVisitor<Visitor, Visitable, ResultType>::get_value(Visitable v) {
        Visitor visitor;
        if (v == nullptr) {
            throw EvaluatorException("Error evaluating expression, unexpected null node.");
        }

        v->accept(visitor);
        return visitor.m_value;
    }

    template<typename Visitor, typename Visitable, typename ResultType>
    void ValueVisitor<Visitor, Visitable, ResultType>::result(ResultType result) {
        m_value = result;
    }

    void Interpreter::visit(Chinstrap::NumberNode &node) {
        result(node.value());
    }

    void Interpreter::visit(BinaryOperationNode &node) {
        auto left = get_value(node.left());
        auto right = get_value(node.right());

        switch (node.type()) {
            case BinaryOperationNode::Type::Addition: RESULT_CASE(left + right)
            case BinaryOperationNode::Type::Subtraction: RESULT_CASE(left - right)
            case BinaryOperationNode::Type::Multiplication: RESULT_CASE(left * right)
            case BinaryOperationNode::Type::Division: RESULT_CASE(left / right)
            case BinaryOperationNode::Type::ShiftLeft: RESULT_CASE(left << right)
            case BinaryOperationNode::Type::ShiftRight: RESULT_CASE(left >> right)
            default:
                // TODO: Throw error when token is not valid for a binary op node
                break;
        }
    }

    void Interpreter::visit(PrefixOperationNode &node) {
        auto child = get_value(node.child());
        switch (node.type()) {
            case TokenType::Minus: RESULT_CASE(-child)
            default:
                // TODO: Throw error when token is not valid for a binary op node
                break;
        }
    }

    void Interpreter::visit(PostfixOperationNode &node) {
        auto child = get_value(node.child());
        switch (node.type()) {
            case TokenType::Exclamation: RESULT_CASE(PostfixOperationNode::factorial(child))
            default:
                break;
        }
    }

    void Interpreter::visit(ListNode &) {

    }

    void PrettyPrinter::visit(NumberNode &node) {
        std::cout << "NumberNode: " << node.value() << std::endl;
    }

    void PrettyPrinter::visit(BinaryOperationNode &node) {
    }

    void PrettyPrinter::visit(PrefixOperationNode &node) {

    }

    void PrettyPrinter::visit(PostfixOperationNode &node) {

    }

    void PrettyPrinter::visit(ListNode &) {

    }
}

