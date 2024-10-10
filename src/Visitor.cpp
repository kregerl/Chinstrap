#include <unordered_map>
#include <stack>
#include <iostream>
#include <complex>
#include "Visitor.h"
#include "ASTNode.h"
#include "Exception.h"
#include "native_function/PrintFunction.h"
#include "native_function/SingleParameterMathFunction.h"

#define RESULT_CASE(x) {result(x); break;}

namespace Chinstrap {
    static std::deque<Scope> make_scope_deque() {
        std::deque<Scope> deque;
        deque.emplace_front();
        return deque;
    }

    static std::deque<Scope> s_scopes = make_scope_deque();
    static std::unordered_map<std::string, std::shared_ptr<NativeFunction>> s_native_functions = {
            {"print", std::make_shared<PrintFunction>()},
            {"sin",   std::make_shared<SingleParameterMathFunction>(static_cast<double (*)(double)>(std::sin))},
            {"cos",   std::make_shared<SingleParameterMathFunction>(static_cast<double (*)(double)>(std::cos))},
            {"tan",   std::make_shared<SingleParameterMathFunction>(static_cast<double (*)(double)>(std::tan))},
            {"asin",  std::make_shared<SingleParameterMathFunction>(static_cast<double (*)(double)>(std::asin))},
            {"acos",  std::make_shared<SingleParameterMathFunction>(static_cast<double (*)(double)>(std::acos))},
            {"atan",  std::make_shared<SingleParameterMathFunction>(static_cast<double (*)(double)>(std::atan))},
            {"sec",   std::make_shared<SingleParameterMathFunction>(
                    [](double n) -> double { return 1 / std::cos(n); })},
            {"csc",   std::make_shared<SingleParameterMathFunction>(
                    [](double n) -> double { return 1 / std::sin(n); })},
            {"cot",   std::make_shared<SingleParameterMathFunction>(
                    [](double n) -> double { return 1 / std::tan(n); })},

    };

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

    void Interpreter::visit(FunctionNode &node) {
        auto function_name = node.name();

        if (s_native_functions.find(function_name) != s_native_functions.end()) {
            std::vector<Returnable> native_parameters;
            for (auto &param: node.parameters()) {
                native_parameters.emplace_back(get_value(param));
            }
            result(s_native_functions.at(function_name)->call(native_parameters));
            return;
        }
    }

    void Interpreter::visit(BraceNode &node) {
        s_scopes.emplace_front();
        auto expressions = node.value();
        for (auto &expression: expressions) {
            auto x = get_value(expression);
            auto y = x;
            // Create a stack of scopes to hold vars and functions
        }

        s_scopes.pop_front();
        result(Noop());
    }

    void Interpreter::visit(IntegerNode &node) {
        result(IntegerLiteral{node.value()});
    }

    void Interpreter::visit(RealNode &node) {
        result(RealLiteral{node.value()});
    }

    static Returnable get_first_variable_in_scope(const std::string &identifier) {
        for (auto it = s_scopes.crbegin(); it != s_scopes.crend(); ++it) {
            if (it->m_variables.find(identifier) == it->m_variables.end()) {
                continue;
            }
            return it->m_variables.at(identifier);
        }
        throw EvaluatorException("Use of undeclared identifier.");
    }

    void Interpreter::visit(IdentifierNode &node) {
        auto identifier = node.value();
        result(get_first_variable_in_scope(identifier));
        // auto &scope = s_scopes.front();
        //
        // if (scope.m_variables.find(identifier) == scope.m_variables.end()) {
        //     throw EvaluatorException("Use of undeclared identifier.");
        // }
        // result(scope.m_variables.at(identifier));
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
        for (auto &child: node.children()) {
            c.emplace_back(get_value(child));
        }
        result(c);
    }

    void Interpreter::visit(AssignmentNode &node) {
        const auto &value = node.identifier();
        auto &scope = s_scopes.front();
        scope.m_variables[value] = get_value(node.rhs());
    }

    void PrettyPrinter::visit(FunctionNode &node) {
    }

    void PrettyPrinter::visit(BraceNode &node) {
        std::cout << "Brace Node" << std::endl;
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
