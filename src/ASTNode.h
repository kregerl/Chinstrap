#ifndef CHINSTRAP_ASTNODE_H
#define CHINSTRAP_ASTNODE_H

#include <cstdint>
#include <memory>
#include <vector>
#include "Visitor.h"
#include "Lexer.h"

namespace Chinstrap {
    class ASTNode {
    public:
        virtual ~ASTNode() = default;

        virtual void accept(Visitor& visitor) = 0;
    };

    class SingleChildNode : public ASTNode {
    public:
        [[nodiscard]] std::shared_ptr<ASTNode> child() const {
            return m_child;
        }

    protected:
        explicit SingleChildNode(std::shared_ptr<ASTNode> child) : m_child(std::move(child)) {}

    private:
        std::shared_ptr<ASTNode> m_child;
    };

    class NumberNode : public ASTNode {
    public:
        explicit NumberNode(double value);

        [[nodiscard]] double value() const { return m_value; }

        void accept(Visitor& visitor) override;

    private:
        double m_value;
    };

    class ListNode: public ASTNode {
    public:
        explicit ListNode(std::vector<std::shared_ptr<ASTNode>> children);

        void accept(Visitor& visitor) override;
    private:
        std::vector<std::shared_ptr<ASTNode>> m_children;
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

        void accept(Visitor& visitor) override;

        [[nodiscard]] std::shared_ptr<ASTNode> left() const {
            return m_left_child;
        }

        [[nodiscard]] std::shared_ptr<ASTNode> right() const {
            return m_right_child;
        }

        [[nodiscard]] Type type() const {
            return m_type;
        }

    private:
        Type m_type;
        std::shared_ptr<ASTNode> m_left_child;
        std::shared_ptr<ASTNode> m_right_child;
    };

    class PrefixOperationNode : public SingleChildNode {
    public:
        PrefixOperationNode(Token token, std::shared_ptr<ASTNode> child);

        [[nodiscard]] TokenType type() const {
            return m_token.type;
        }

        void accept(Visitor& visitor) override;

    private:
        Token m_token;
    };

    class PostfixOperationNode : public SingleChildNode {
    public:
        PostfixOperationNode(std::shared_ptr<ASTNode> child, Token token);

        [[nodiscard]] TokenType type() const {
            return m_token.type;
        }

        void accept(Visitor& visitor) override;

        static int64_t factorial(int64_t n);

    private:
        Token m_token;
    };

}

#endif //CHINSTRAP_ASTNODE_H
