#ifndef CHINSTRAP_ASTNODE_H
#define CHINSTRAP_ASTNODE_H

#include <cstdint>
#include <memory>
#include "Visitor.h"
#include "Lexer.h"

namespace Chinstrap {
    class ASTNode {
    public:
        virtual ~ASTNode() = default;

        virtual int64_t accept(Visitor& visitor) = 0;
    };

    class IntegerNode : public ASTNode {
    public:
        explicit IntegerNode(uint64_t value);

        [[nodiscard]] uint64_t value() const { return m_value; }

        int64_t accept(Visitor& visitor) override;

    private:
        int64_t m_value;
    };

    class BinaryOperationNode : public ASTNode {
    public:
        BinaryOperationNode(TokenType op_type, std::shared_ptr<ASTNode> left_child,
                            std::shared_ptr<ASTNode> right_child);

        int64_t accept(Visitor& visitor) override;

    private:
        TokenType m_type;
        std::shared_ptr<ASTNode> m_left_child;
        std::shared_ptr<ASTNode> m_right_child;
    };

    class PrefixOperationNode : public ASTNode {
    public:
        PrefixOperationNode(TokenType op_type, std::shared_ptr<ASTNode> child);

        int64_t accept(Visitor& visitor) override;

    private:
        TokenType m_type;
        std::shared_ptr<ASTNode> m_child;
    };

    class PostfixOperationNode : public ASTNode {
    public:
        PostfixOperationNode(std::shared_ptr<ASTNode> child, TokenType op_type);

        int64_t accept(Visitor& visitor) override;

    private:
        TokenType m_type;
        std::shared_ptr<ASTNode> m_child;
    };

}

#endif //CHINSTRAP_ASTNODE_H
