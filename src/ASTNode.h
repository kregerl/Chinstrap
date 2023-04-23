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

        virtual void accept_print(Visitor& visitor, int* indent_amount) = 0;
    };

    class IntegerNode : public ASTNode {
    public:
        explicit IntegerNode(uint64_t value);

        [[nodiscard]] uint64_t value() const { return m_value; }

        int64_t accept(Visitor& visitor) override;

        void accept_print(Visitor& visitor, int* indent_amount) override;

    private:
        int64_t m_value;
    };

    class BinaryOperationNode : public ASTNode {
    public:
        enum class Type {
            Addition,
            Subtraction,
            Multiplication,
            Division,
            Modulus,
            And,
            Or,
            ShiftLeft,
            ShiftRight
        };

        static std::optional<Type> op_type_from_token_type(const TokenType& type) {
            switch (type) {
                case TokenType::Plus: return Type::Addition;
                case TokenType::Minus:return Type::Subtraction;
                case TokenType::Asterisk:return Type::Multiplication;
                case TokenType::Slash:return Type::Division;
//                case TokenType::LessThan:break;
//                case TokenType::GreaterThan:break;
//                case TokenType::Pipe:break;
//                case TokenType::Ampersand:break;
                default: return {};
            }
        }

    public:
        BinaryOperationNode(Type op_type, std::shared_ptr<ASTNode> left_child,
                            std::shared_ptr<ASTNode> right_child);

        int64_t accept(Visitor& visitor) override;

        void accept_print(Visitor& visitor, int* indent_amount) override;

    private:
        Type m_type;
        std::shared_ptr<ASTNode> m_left_child;
        std::shared_ptr<ASTNode> m_right_child;
    };

    class PrefixOperationNode : public ASTNode {
    public:
        PrefixOperationNode(Token token, std::shared_ptr<ASTNode> child);

        int64_t accept(Visitor& visitor) override;

        void accept_print(Visitor& visitor, int* indent_amount) override;

    private:
        Token m_token;
        std::shared_ptr<ASTNode> m_child;
    };

    class PostfixOperationNode : public ASTNode {
    public:
        PostfixOperationNode(std::shared_ptr<ASTNode> child, Token token);

        int64_t accept(Visitor& visitor) override;

        void accept_print(Visitor& visitor, int* indent_amount) override;

    private:
        static int64_t factorial(int64_t n);

    private:
        Token m_token;
        std::shared_ptr<ASTNode> m_child;
    };

}

#endif //CHINSTRAP_ASTNODE_H
