#ifndef CHINSTRAP_VISITOR_H
#define CHINSTRAP_VISITOR_H

#include <cstdint>

namespace Chinstrap {
    class IntegerNode;

    class BinaryOperationNode;

    class PrefixOperationNode;

    class PostfixOperationNode;

    class Visitor {
    public:
        virtual int64_t visit(IntegerNode&) = 0;

        virtual int64_t visit(BinaryOperationNode&) = 0;

        virtual int64_t visit(PrefixOperationNode&) = 0;

        virtual int64_t visit(PostfixOperationNode&) = 0;
    };

    class Interpreter : public Visitor {
    public:
        int64_t visit(IntegerNode&) override;

        int64_t visit(BinaryOperationNode&) override;

        int64_t visit(PrefixOperationNode&) override;

        int64_t visit(PostfixOperationNode&) override;

    private:
        int64_t m_result = 0;
    };

    class PrettyPrinter : public Visitor {
    public:
        int64_t visit(IntegerNode&) override;

        int64_t visit(BinaryOperationNode&) override;

        int64_t visit(PrefixOperationNode&) override;

        int64_t visit(PostfixOperationNode&) override;
    private:
        int m_indent_amount = 0;
    };
}

#endif //CHINSTRAP_VISITOR_H
