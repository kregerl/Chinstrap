#ifndef CHINSTRAP_ASTNODE_H
#define CHINSTRAP_ASTNODE_H

#include <cstdint>
#include <memory>
#include <vector>
#include "Visitor.h"
#include "Lexer.h"

#define MAKE_VISITABLE void accept(Visitor &visitor) override { visitor.visit(*this); }

namespace Chinstrap {
    class ASTNode {
    public:
        virtual ~ASTNode() = default;

        virtual void accept(Visitor &visitor) = 0;
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

    template<typename T>
    class ValueNode : public ASTNode {
    public:
        explicit ValueNode(T value) : m_value(std::move(value)) {}

        [[nodiscard]] T value() const { return m_value; }

    protected:
        T m_value;
    };

    class IntegerNode : public ValueNode<int64_t> {
        MAKE_VISITABLE

    public:
        explicit IntegerNode(int64_t value);
    };

    class RealNode : public ValueNode<double> {
        MAKE_VISITABLE

    public:
        explicit RealNode(double value);
    };

    class ListNode : public ASTNode {
        MAKE_VISITABLE

    public:
        explicit ListNode(std::vector<std::shared_ptr<ASTNode>> children);

        [[nodiscard]] const std::vector<std::shared_ptr<ASTNode>> &children() const { return m_children; }

    private:
        std::vector<std::shared_ptr<ASTNode>> m_children;
    };

    class BinaryOperationNode : public ASTNode {
        MAKE_VISITABLE

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

        static std::optional<Type> op_type_from_token_type(const TokenType &type) {
            switch (type) {
                case TokenType::Plus:
                    return Type::Addition;
                case TokenType::Minus:
                    return Type::Subtraction;
                case TokenType::Asterisk:
                    return Type::Multiplication;
                case TokenType::Slash:
                    return Type::Division;
                case TokenType::Percent:
                    return Type::Modulus;
                case TokenType::Pipe:
                    return Type::Or;
                case TokenType::Ampersand:
                    return Type::And;
                default:
                    return {};
            }
        }

    public:
        BinaryOperationNode(Type op_type, std::shared_ptr<ASTNode> left_child,
                            std::shared_ptr<ASTNode> right_child);

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
        MAKE_VISITABLE

    public:
        PrefixOperationNode(Token token, std::shared_ptr<ASTNode> child);

        [[nodiscard]] TokenType type() const {
            return m_token.type;
        }

    private:
        Token m_token;
    };

    class PostfixOperationNode : public SingleChildNode {
        MAKE_VISITABLE

    public:
        PostfixOperationNode(std::shared_ptr<ASTNode> child, Token token);

        [[nodiscard]] TokenType type() const {
            return m_token.type;
        }

        static IntegerLiteral factorial(IntegerLiteral literal);

    private:
        Token m_token;
    };

}

#endif //CHINSTRAP_ASTNODE_H
