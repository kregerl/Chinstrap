#ifndef CHINSTRAP_VISITOR_H
#define CHINSTRAP_VISITOR_H

#include <cstdint>
#include <memory>
#include <variant>
#include <string>
#include <unordered_map>
#include "types/Literal.h"
#include "NodeForward.h"
#include "types/Collection.h"
#include "Visit.h"
#include "native_function/Function.h"
#include "Returnable.h"

#define UNORDERED_VISIT(type1, type2, expression) \
                        [](type1 &lhs, type2 &rhs) -> Returnable { return expression; },\
                        [](type2 &lhs, type1 &rhs) -> Returnable { return expression; },

namespace Chinstrap {
    class Visitor {
    public:
        virtual void visit(IntegerNode &) = 0;
        virtual void visit(RealNode &) = 0;
        virtual void visit(ListNode &) = 0;
        virtual void visit(IdentifierNode &) = 0;
        virtual void visit(BinaryOperationNode &) = 0;
        virtual void visit(PrefixOperationNode &) = 0;
        virtual void visit(PostfixOperationNode &) = 0;
        virtual void visit(AssignmentNode &) = 0;
        virtual void visit(BraceNode &) = 0;
        virtual void visit(FunctionNode &) = 0;
        virtual void visit(FunctionDefinitionNode &) = 0;
    };

  
    template<typename Visitor, typename Visitable, typename ResultType>
    class ValueVisitor {
    public:
        ValueVisitor() = default;

        static ResultType get_value(Visitable v);

        static void evaluate(Visitable v);

        virtual void result(ResultType result);

    protected:
        ResultType m_value;
    };

    struct Scope {
        std::unordered_map<std::string, Returnable> m_variables; 
        std::unordered_map<std::string, Function> m_functions;

        Scope(): m_variables({}) {}
    };


    class Interpreter : public Visitor, public ValueVisitor<Interpreter, std::shared_ptr<ASTNode>, Returnable> {
    public:
        void visit(IntegerNode &) override;
        void visit(RealNode &) override;
        void visit(ListNode &) override;
        void visit(IdentifierNode &) override;
        void visit(BinaryOperationNode &) override;
        void visit(PrefixOperationNode &) override;
        void visit(PostfixOperationNode &) override;
        void visit(AssignmentNode &) override;
        void visit(BraceNode &) override;
        void visit(FunctionNode &) override;
        void visit(FunctionDefinitionNode &) override;
    };

    class PrettyPrinter : public Visitor, public ValueVisitor<Interpreter, std::shared_ptr<ASTNode>, Returnable> {
    public:
        void visit(IntegerNode &) override;
        void visit(RealNode &) override;
        void visit(IdentifierNode &) override;
        void visit(ListNode &) override;
        void visit(BinaryOperationNode &) override;
        void visit(PrefixOperationNode &) override;
        void visit(PostfixOperationNode &) override;
        void visit(AssignmentNode &) override;
        void visit(BraceNode &) override;
        void visit(FunctionNode &) override;
        void visit(FunctionDefinitionNode &) override;
    private:
        int m_indent_amount = 0;
    };
}

#endif //CHINSTRAP_VISITOR_H
