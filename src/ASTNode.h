#ifndef CHINSTRAP_ASTNODE_H
#define CHINSTRAP_ASTNODE_H

#include <cstdint>
#include <memory>
#include <vector>
#include "Visitor.h"
#include "Lexer.h"

#define MAKE_VISITABLE void accept(Visitor &visitor) override { visitor.visit(*this); }

namespace Chinstrap {
    template<typename T>
    class VisitedBy {
    public:
        virtual ~VisitedBy() = default;

    private:
        virtual void accept(T &visitor) = 0;
    };

    class ASTNode : public VisitedBy<Visitor> {
    public:
        virtual ~ASTNode() = default;

        void accept(Visitor &visitor) override = 0;
    };

    class SingleChildNode : public ASTNode {
    public:
        [[nodiscard]] std::shared_ptr<ASTNode> child() const {
            return m_child;
        }

    protected:
        explicit SingleChildNode(std::shared_ptr<ASTNode> child) : m_child(std::move(child)) {
        }

    private:
        std::shared_ptr<ASTNode> m_child;
    };

    template<typename T>
    class ValueNode : public ASTNode {
    public:
        explicit ValueNode(T value) : m_value(std::move(value)) {
        }

        [[nodiscard]] T value() const { return m_value; }

    protected:
        T m_value;
    };

    class BraceNode : public ValueNode<std::vector<std::shared_ptr<ASTNode> > > {
        MAKE_VISITABLE

    public:
        explicit BraceNode(const std::vector<std::shared_ptr<ASTNode> > &value);
    };

    class FunctionNode : public ASTNode {
        MAKE_VISITABLE

    public:
        FunctionNode(std::string name, const std::vector<std::shared_ptr<ASTNode> > &value);

        [[nodiscard]] std::vector<std::shared_ptr<ASTNode> > &parameters() { return m_parameters; };

        [[nodiscard]] const std::string &name() const { return m_name; };

    private:
        std::vector<std::shared_ptr<ASTNode> > m_parameters;
        std::string m_name;
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

    class IdentifierNode : public ValueNode<std::string> {
        MAKE_VISITABLE

    public:
        explicit IdentifierNode(std::string value);
    };

    class ListNode : public ASTNode {
        MAKE_VISITABLE

    public:
        explicit ListNode(std::vector<std::shared_ptr<ASTNode> > children);

        [[nodiscard]] const std::vector<std::shared_ptr<ASTNode> > &children() const { return m_children; }

    private:
        std::vector<std::shared_ptr<ASTNode> > m_children;
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

    class AssignmentNode : public ASTNode {
        MAKE_VISITABLE

    public:
        AssignmentNode(std::string identifier, std::shared_ptr<ASTNode> rhs);

        [[nodiscard]] const std::string &identifier() const { return m_identifier; }

        [[nodiscard]] std::shared_ptr<ASTNode> rhs() const { return m_rhs; }

    private:
        std::string m_identifier;
        std::shared_ptr<ASTNode> m_rhs;
    };

    class FunctionDefinitionNode : public ASTNode {
        MAKE_VISITABLE

    public:
        FunctionDefinitionNode(std::string identifier, std::vector<Token> parameters, std::shared_ptr<ASTNode> body);

        [[nodiscard]] const std::string &identifier() const { return m_identifier; }

        [[nodiscard]] const std::vector<Token> &parameters() const { return m_parameters; }

        [[nodiscard]] std::shared_ptr<ASTNode> body() const { return m_body; }
    private:
        std::string m_identifier;
        std::vector<Token> m_parameters;
        std::shared_ptr<ASTNode> m_body;
    };
}

#endif //CHINSTRAP_ASTNODE_H
