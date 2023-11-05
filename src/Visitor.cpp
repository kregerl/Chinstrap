#include <unordered_map>
#include <iostream>
#include "Visitor.h"
#include "ASTNode.h"
#include "Exception.h"

#define RESULT_CASE(x) {result(x); break;}

namespace Chinstrap {

    static std::unordered_map<std::string, Returnable> s_variables = {};

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

    void Interpreter::visit(IntegerNode &node) {
        result(IntegerLiteral{node.value()});
    }

    void Interpreter::visit(RealNode &node) {
        result(RealLiteral{node.value()});
    }

    void Interpreter::visit(IdentifierNode &node) {
        auto value = node.value();
        if (s_variables.find(value) == s_variables.end()) {
            throw EvaluatorException("Use of undeclared identifier.");
        }
        result(s_variables.at(value));
    }

    void Interpreter::visit(BinaryOperationNode &node) {
        auto left = get_value(node.left());
        auto right = get_value(node.right());

        switch (node.type()) {
            case BinaryOperationNode::Type::Addition: {
                RESULT_CASE(std::visit(overloaded{
                        [](IntegerLiteral &lhs, IntegerLiteral &rhs) -> Returnable { return lhs + rhs; },
                        [](RealLiteral &lhs, RealLiteral &rhs) -> Returnable { return lhs + rhs; },
                        UNORDERED_VISIT(IntegerLiteral, RealLiteral, RealLiteral(lhs.get_value() + rhs.get_value()))
                        [](auto & , auto & )->Returnable {
                            throw EvaluatorException("Invalid operation for binary operation node.");
                        }
                }, left, right))
            }
            case BinaryOperationNode::Type::Subtraction: {
                RESULT_CASE(std::visit(overloaded{
                        [](IntegerLiteral &lhs, IntegerLiteral &rhs) -> Returnable { return lhs - rhs; },
                        [](RealLiteral &lhs, RealLiteral &rhs) -> Returnable { return lhs - rhs; },
                        UNORDERED_VISIT(IntegerLiteral, RealLiteral, RealLiteral(lhs.get_value() - rhs.get_value()))
                        [](auto & , auto & )->Returnable {
                            throw EvaluatorException("Invalid operation for binary operation node.");
                        }
                }, left, right))
            }
            case BinaryOperationNode::Type::Multiplication: {
                RESULT_CASE(std::visit(overloaded{
                        [](IntegerLiteral &lhs, IntegerLiteral &rhs) -> Returnable { return lhs * rhs; },
                        [](RealLiteral &lhs, RealLiteral &rhs) -> Returnable { return lhs * rhs; },
                        UNORDERED_VISIT(IntegerLiteral, RealLiteral, RealLiteral(lhs.get_value() * rhs.get_value()))
                        [](auto & , auto & )->Returnable {
                            throw EvaluatorException("Invalid operation for binary operation node.");
                        }
                }, left, right))
            }
            case BinaryOperationNode::Type::Division: {
                RESULT_CASE(std::visit(overloaded{
                        [](IntegerLiteral &lhs, IntegerLiteral &rhs) -> Returnable {
                            if (rhs.get_value() == 0)
                                throw EvaluatorException("Division by 0");
                            return lhs / rhs;
                        },
                        [](RealLiteral &lhs, RealLiteral &rhs) -> Returnable {
                            if (rhs.get_value() == 0)
                                throw EvaluatorException("Division by 0");
                            return lhs / rhs;
                        },
                        [](IntegerLiteral &lhs, RealLiteral &rhs) -> Returnable {
                            if (rhs.get_value() == 0)
                                throw EvaluatorException("Division by 0");
                            return RealLiteral(lhs.get_value() / rhs.get_value());
                        },
                        [](RealLiteral &lhs, IntegerLiteral &rhs) -> Returnable {
                            if (rhs.get_value() == 0)
                                throw EvaluatorException("Division by 0");
                            return RealLiteral(lhs.get_value() / rhs.get_value());
                        },
                        [](auto &, auto &) -> Returnable {
                            throw EvaluatorException("Invalid operation for binary operation node.");
                        }
                }, left, right))
            }
            case BinaryOperationNode::Type::ShiftLeft: {
                RESULT_CASE(std::visit(overloaded{
                        [](IntegerLiteral &lhs, IntegerLiteral &rhs) -> Returnable { return lhs << rhs; },
                        [](auto &, auto &) -> Returnable {
                            throw EvaluatorException("Invalid operation for binary operation node.");
                        }
                }, left, right))
            }
            case BinaryOperationNode::Type::ShiftRight: {
                RESULT_CASE(std::visit(overloaded{
                        [](IntegerLiteral &lhs, IntegerLiteral &rhs) -> Returnable { return lhs >> rhs; },
                        [](auto &, auto &) -> Returnable {
                            throw EvaluatorException("Invalid operation for binary operation node.");
                        }
                }, left, right))
            }
            case BinaryOperationNode::Type::Modulus: {
                RESULT_CASE(std::visit(overloaded{
                        [](IntegerLiteral &lhs, IntegerLiteral &rhs) -> Returnable { return lhs % rhs; },
                        [](auto &, auto &) -> Returnable {
                            throw EvaluatorException("Invalid operation for binary operation node.");
                        }
                }, left, right))
            }
            case BinaryOperationNode::Type::And: {
                RESULT_CASE(std::visit(overloaded{
                        [](IntegerLiteral &lhs, IntegerLiteral &rhs) -> Returnable { return lhs & rhs; },
                        [](auto &, auto &) -> Returnable {
                            throw EvaluatorException("Invalid operation for binary operation node.");
                        }
                }, left, right))
            }
            case BinaryOperationNode::Type::Or: {
                RESULT_CASE(std::visit(overloaded{
                        [](IntegerLiteral &lhs, IntegerLiteral &rhs) -> Returnable { return lhs | rhs; },
                        [](auto &, auto &) -> Returnable {
                            throw EvaluatorException("Invalid operation for binary operation node.");
                        }
                }, left, right))
            }
            default:
                throw EvaluatorException("Unimplemented binary operation type.");
        }
    }

    void Interpreter::visit(PrefixOperationNode &node) {
        auto child = get_value(node.child());
        switch (node.type()) {
            case TokenType::Minus: {
                RESULT_CASE(std::visit(overloaded{
                        [](IntegerLiteral &child) -> Returnable { return child * IntegerLiteral(-1); },
                        [](RealLiteral &child) -> Returnable { return child * RealLiteral(-1); },
                        [](auto &) -> Returnable {
                            throw EvaluatorException("Invalid operation for binary operation node.");
                        }
                }, child))
            }
            default:
                throw EvaluatorException("Unimplemented unary operation type.");
        }
    }

    void Interpreter::visit(PostfixOperationNode &node) {
        auto child = get_value(node.child());

        switch (node.type()) {
            case TokenType::Exclamation: {
                RESULT_CASE(std::visit(overloaded{
                        [](IntegerLiteral &child) -> Returnable { return PostfixOperationNode::factorial(child); },
                        [](auto &) -> Returnable {
                            throw EvaluatorException("Invalid operation for binary operation node.");
                        }
                }, child))
            }
            default:
                throw EvaluatorException("Unimplemented unary operation type.");
        }
    }

    void Interpreter::visit(ListNode &node) {
        Collection c;
        for (auto& child : node.children()) {
            c.emplace_back(get_value(child));
        }
        result(c);
    }

    void Interpreter::visit(AssignmentNode &node) {
        const auto& value = node.identifier();
        s_variables[value] = get_value(node.rhs());
    }

    void PrettyPrinter::visit(IntegerNode &node) {
        std::cout << "IntegerNode: " << node.value() << std::endl;
    }

    void PrettyPrinter::visit(RealNode &node) {
        std::cout << "RealNode: " << node.value() << std::endl;
    }

    void PrettyPrinter::visit(BinaryOperationNode &node) {
    }

    void PrettyPrinter::visit(PrefixOperationNode &node) {

    }

    void PrettyPrinter::visit(PostfixOperationNode &node) {

    }

    void PrettyPrinter::visit(ListNode &) {

    }

    void PrettyPrinter::visit(IdentifierNode &) {

    }

    void PrettyPrinter::visit(AssignmentNode &) {

    }

}

