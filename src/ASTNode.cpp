#include <iostream>
#include <cstdlib>
#include <utility>
#include "ASTNode.h"

namespace Chinstrap {
    FunctionNode::FunctionNode(std::string name, const std::vector<std::shared_ptr<ASTNode>> &parameters)
            : m_name(std::move(name)), m_parameters(parameters) {}

    BraceNode::BraceNode(const std::vector<std::shared_ptr<ASTNode>> &value, bool push_scope)
            : ValueNode(value), m_push_scope(push_scope) {}

    IntegerNode::IntegerNode(int64_t value) : ValueNode(value) {}

    RealNode::RealNode(double value) : ValueNode(value) {}

    IdentifierNode::IdentifierNode(std::string value) : ValueNode(std::move(value)) {}

    ListNode::ListNode(std::vector<std::shared_ptr<ASTNode>> children) : m_children(std::move(children)) {}

    BinaryOperationNode::BinaryOperationNode(
            BinaryOperationNode::Type op_type,
            std::shared_ptr<ASTNode> left_child,
            std::shared_ptr<ASTNode> right_child)
            : m_type(op_type),
              m_left_child(std::move(left_child)),
              m_right_child(std::move(right_child)) {}


    PrefixOperationNode::PrefixOperationNode(Token token, std::shared_ptr<ASTNode> child)
            : m_token(std::move(token)), SingleChildNode(std::move(child)) {}

    PostfixOperationNode::PostfixOperationNode(std::shared_ptr<ASTNode> child, Token token)
            : SingleChildNode(std::move(child)), m_token(std::move(token)) {}

    AssignmentNode::AssignmentNode(std::string identifier, std::shared_ptr<ASTNode> rhs) :
            m_rhs(std::move(rhs)), m_identifier(std::move(identifier)) {}

    FunctionDefinitionNode::FunctionDefinitionNode(
            std::string identifier,
            std::vector<Token> parameters,
            std::shared_ptr<ASTNode> body)
            : m_identifier(std::move(identifier)), m_parameters(std::move(parameters)), m_body(std::move(body)) {}
}

