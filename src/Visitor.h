#ifndef CHINSTRAP_VISITOR_H
#define CHINSTRAP_VISITOR_H

#include <cstdint>
#include <memory>
#include <variant>
#include "Literal.h"

namespace Chinstrap {
    class ASTNode;

    class NumberNode;

    class ListNode;

    class BinaryOperationNode;

    class PrefixOperationNode;

    class PostfixOperationNode;

    class Visitor {
    public:
        virtual void visit(NumberNode &) = 0;

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

    template<class... Ts>
    struct overload : Ts ... {
        using Ts::operator()...;
    };

    template<class... Ts> overload(Ts...) -> overload<Ts...>;

    using Returnable = std::variant<NumericLiteral, StringLiteral>;

    class Interpreter : public Visitor, public ValueVisitor<Interpreter, std::shared_ptr<ASTNode>, int64_t> {
    public:
        void visit(NumberNode &) override;

        void visit(ListNode &) override;

        void visit(BinaryOperationNode &) override;

        void visit(PrefixOperationNode &) override;

        void visit(PostfixOperationNode &) override;
    };

    class PrettyPrinter : public Visitor, public ValueVisitor<Interpreter, std::shared_ptr<ASTNode>, int64_t> {
    public:
        void visit(NumberNode &) override;

        void visit(ListNode &) override;

        void visit(BinaryOperationNode &) override;

        void visit(PrefixOperationNode &) override;

        void visit(PostfixOperationNode &) override;

    private:
        int m_indent_amount = 0;
    };
}

#endif //CHINSTRAP_VISITOR_H
