#include "Parser.h"
#include "parslet/GroupParslet.h"

namespace Chinstrap {

    Parser::Parser(std::string_view source) : m_lexer(source) {
        m_prefix_parslets = {
                {TokenType::Integer, new PrefixParslet()},
                {TokenType::Minus,   new PrefixParslet(Precedence::Prefix)},
                {TokenType::LParen,  new GroupParslet()}
        };
        m_infix_parslets = {
                {TokenType::Plus,     new InfixParslet(Precedence::Sum)},
                {TokenType::Minus,    new InfixParslet(Precedence::Sum)},
                {TokenType::Asterisk, new InfixParslet(Precedence::Product)},
                {TokenType::Slash,    new InfixParslet(Precedence::Product)}
        };
        next();
    }

    Parser::~Parser() {
        // Clean up heap allocated parselets.
        for (auto& entry: m_prefix_parslets) {
            delete entry.second;
        }
        for (auto& entry: m_infix_parslets) {
            delete entry.second;
        }
    }

    std::shared_ptr<ASTNode> Parser::parse_expression(Precedence precedence) {
        Token token = m_current;
        next();

        if (token.type == TokenType::LexError) {
            throw std::runtime_error("Error lexing token");
        }

        // If the current token type isn't in m_prefix_parslets, throw error.
        if (m_prefix_parslets.find(token.type) == m_prefix_parslets.end()) {
            std::stringstream ss;
            ss << "Could not parse prefix token '";
            ss << token.value;
            ss << "'";
            throw std::runtime_error(ss.str());
        }
        // Otherwise, get the parslet and parse an ASTNode from it.
        auto prefix = m_prefix_parslets.at(token.type);
        std::shared_ptr<ASTNode> node = prefix->parse(*this, token);

        // Continue to parse while the precedence is lower than the next.
        while (precedence < get_precedence()) {
            token = m_current;
            next();

            auto infix = m_infix_parslets.at(token.type);
            node = infix->parse(*this, token, node);
        }

        return node;
    }

    Token Parser::consume(const TokenType& type) {
        Token token = m_current;
        next();
        if (token.type != type) {
            std::stringstream ss;
            ss << "Expected token type '";
            ss << (int) type;
            ss << "' but got '";
            ss << token.value;
            ss << "'";
            throw std::runtime_error(ss.str());
        }
        return token;
    }

    void Parser::next() {
        if (!m_lexer.is_end())
            m_current = m_lexer.next();
    }

    Precedence Parser::get_precedence() {
        if (m_infix_parslets.find(m_current.type) != m_infix_parslets.end()) {
            return m_infix_parslets.at(m_current.type)->get_precedence();
        }
        return Precedence::None;
    }

}



