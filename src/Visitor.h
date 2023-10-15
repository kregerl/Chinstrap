#ifndef CHINSTRAP_VISITOR_H
#define CHINSTRAP_VISITOR_H

#include <cstdint>
#include <memory>
#include <variant>
#include "types/Literal.h"
#include "NodeForward.h"
#include "types/Collection.h"
#include "Visit.h"

#define UNORDERED_VISIT(type1, type2, expression) \
                        [](type1 &lhs, type2 &rhs) -> Returnable { return expression; },\
                        [](type2 &lhs, type1 &rhs) -> Returnable { return expression; },

namespace Chinstrap {
    class Visitor {
    public:
        virtual void visit(IntegerNode &) = 0;

        virtual void visit(RealNode &) = 0;

        virtual void visit(ListNode &) = 0;

        virtual void visit(BinaryOperationNode &) = 0;

        virtual void visit(PrefixOperationNode &) = 0;

        virtual void visit(PostfixOperationNode &) = 0;
    };

    template<typename Visitor, typename Visitable, typename ResultType>
    class ValueVisitor {
    public:
        ValueVisitor() = default;

        static ResultType get_value(Visitable v);

        virtual void result(ResultType result);

    protected:
        ResultType m_value;
    };

    using Returnable = std::variant<IntegerLiteral, RealLiteral, Collection>;

    class Interpreter : public Visitor, public ValueVisitor<Interpreter, std::shared_ptr<ASTNode>, Returnable> {
    public:
        void visit(IntegerNode &) override;

        void visit(RealNode &) override;

        void visit(ListNode &) override;

        void visit(BinaryOperationNode &) override;

        void visit(PrefixOperationNode &) override;

        void visit(PostfixOperationNode &) override;
    };

    class PrettyPrinter : public Visitor, public ValueVisitor<Interpreter, std::shared_ptr<ASTNode>, Returnable> {
    public:
        void visit(IntegerNode &) override;

        void visit(RealNode &) override;

        void visit(ListNode &) override;

        void visit(BinaryOperationNode &) override;

        void visit(PrefixOperationNode &) override;

        void visit(PostfixOperationNode &) override;

    private:
        int m_indent_amount = 0;
    };
}

#endif //CHINSTRAP_VISITOR_H
