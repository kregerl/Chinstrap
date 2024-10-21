#include <unordered_map>
#include <stack>
#include <iostream>
#include <complex>
#include "Visitor.h"
#include "ASTNode.h"
#include "Exception.h"
#include "function/PrintFunction.h"
#include "function/SingleParameterMathFunction.h"
#include "util/StringFormat.h"
#include "types/OperationVisitor.h"

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
    void ValueVisitor<Visitor, Visitable, ResultType>::evaluate(Visitable v) {
        Visitor visitor;
        if (v == nullptr) {
            throw EvaluatorException("Error evaluating expression, unexpected null node.");
        }

        v->accept(visitor);
    }

    template<typename Visitor, typename Visitable, typename ResultType>
    void ValueVisitor<Visitor, Visitable, ResultType>::result(ResultType result) {
        m_value = result;
    }

    void Interpreter::visit(IntegerNode &node) {
        result(node.value());
    }

    void Interpreter::visit(RealNode &node) {
        result(node.value());
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
    }

    void Interpreter::visit(BinaryOperationNode &node) {
        auto left = get_value(node.left());
        auto right = get_value(node.right());

        switch (node.type()) {
            case BinaryOperationNode::Type::Addition: {
                RESULT_CASE(std::visit(NumericBinaryOperationVisitor{std::plus<>{}}, left, right))
            }
            case BinaryOperationNode::Type::Subtraction: {
                RESULT_CASE(std::visit(NumericBinaryOperationVisitor{std::plus<>{}}, left, right))
            }
            case BinaryOperationNode::Type::Multiplication: {
                RESULT_CASE(std::visit(NumericBinaryOperationVisitor{std::multiplies<>{}}, left, right))
            }
            case BinaryOperationNode::Type::Division: {
                RESULT_CASE(std::visit(DivisionVisitor{}, left, right))
            }
            case BinaryOperationNode::Type::ShiftLeft: {
                RESULT_CASE(std::visit(IntegerBinaryOperationVisitor{
                        [](auto lhs, auto rhs) {
                            return lhs << rhs;
                        }}, left, right))
            }
            case BinaryOperationNode::Type::ShiftRight: {
                RESULT_CASE(std::visit(IntegerBinaryOperationVisitor{
                        [](auto lhs, auto rhs) {
                            return lhs >> rhs;
                        }}, left, right))
            }
            case BinaryOperationNode::Type::Modulus: {
                RESULT_CASE(std::visit(IntegerBinaryOperationVisitor{std::modulus<>{}}, left, right))
            }
            case BinaryOperationNode::Type::And: {
                RESULT_CASE(std::visit(IntegerBinaryOperationVisitor{std::bit_and<>{}}, left, right))
            }
            case BinaryOperationNode::Type::Or: {
                RESULT_CASE(std::visit(IntegerBinaryOperationVisitor{std::bit_or<>{}}, left, right))
            }
            case BinaryOperationNode::Type::GreaterThan: {
                RESULT_CASE(std::visit(NumericBinaryOperationVisitor{std::greater<>{}}, left, right))
            }
            case BinaryOperationNode::Type::GreaterThanEqual: {
                RESULT_CASE(std::visit(NumericBinaryOperationVisitor{std::greater_equal<>{}}, left, right))
            }
            case BinaryOperationNode::Type::LessThan: {
                RESULT_CASE(std::visit(NumericBinaryOperationVisitor{std::less<>{}}, left, right))
            }
            case BinaryOperationNode::Type::LessThanEqual: {
                RESULT_CASE(std::visit(NumericBinaryOperationVisitor{std::less_equal<>{}}, left, right))
            }
            case BinaryOperationNode::Type::EqualTo: {
                RESULT_CASE(std::visit(NumericBinaryOperationVisitor{std::equal_to<>{}}, left, right))
            }
            case BinaryOperationNode::Type::NotEqualTo: {
                RESULT_CASE(std::visit(NumericBinaryOperationVisitor{std::not_equal_to<>{}}, left, right))
            }
            default:
                throw EvaluatorException("Unimplemented binary operation m_type.");
        }
    }

    void Interpreter::visit(PrefixOperationNode &node) {
        auto child = get_value(node.child());
        switch (node.type()) {
            case TokenType::Minus: {
                Returnable inverse = -1;
                RESULT_CASE(std::visit(NumericBinaryOperationVisitor{std::multiplies<>{}}, child, inverse))
            }
            default:
                throw EvaluatorException("Unimplemented unary operation m_type.");
        }
    }

    void Interpreter::visit(PostfixOperationNode &node) {
        auto child = get_value(node.child());

        switch (node.type()) {
            case TokenType::Exclamation: {
                RESULT_CASE(
                        std::visit(IntegerUnaryOperationVisitor{[](auto value) { return Operation::factorial(value); }}, child))
            }
            default:
                throw EvaluatorException("Unimplemented unary operation m_type.");
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

    void Interpreter::visit(BraceNode &node) {
        if (node.push_scope())
            s_scopes.emplace_front();

        auto expressions = node.value();
        for (auto &expression: expressions) {
            evaluate(expression);
        }

        if (node.push_scope())
            s_scopes.pop_front();
        result(Noop());
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
        auto &scope = s_scopes.front();
        const auto &node_name = node.name();
        if (scope.m_functions.find(node_name) != scope.m_functions.end()) {
            auto &function = scope.m_functions.at(node_name);

            auto num_defined_parameters = function.parameters().size();
            auto num_parameters = node.parameters().size();

            if (num_defined_parameters != num_parameters) {
                throw EvaluatorException(
                        string_format("Expected %ld parameters but got %ld", num_defined_parameters, num_parameters));
            }

            Scope function_scope;
            for (int i = 0; i < num_parameters; i++) {
                auto value = get_value(node.parameters().at(i));
                auto identifier = function.parameters().at(i).m_value;
                function_scope.m_variables.emplace(identifier, value);
            }
            s_scopes.emplace_front(function_scope);

            Returnable result_value = Noop{};
            try {
                result_value = get_value(function.body());
            } catch (ReturnValueException &return_value) {
                result_value = return_value.value();
            }
            result(result_value);
            s_scopes.pop_front();
        }
    }

    void Interpreter::visit(FunctionDefinitionNode &node) {
        auto &scope = s_scopes.front();
        scope.m_functions.emplace(node.identifier(), Function(node.parameters(), node.body()));
        result(Noop{});
    }

    void Interpreter::visit(ReturnNode &node) {
        auto value = get_value(node.child());
        throw ReturnValueException(value);
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

    void PrettyPrinter::visit(FunctionDefinitionNode &) {

    }

    void PrettyPrinter::visit(ReturnNode &) {

    }
}
