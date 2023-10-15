#include "Parser.h"
#include "parslet/GroupParslet.h"
#include "parslet/PostfixParslet.h"
#include "parslet/ListParslet.h"

namespace Chinstrap {

    Parser::Parser(std::string_view source) : m_lexer(source) {
        m_prefix_parslets = {
                {TokenType::Integer,   new PrefixParslet()},
                {TokenType::Real,   new PrefixParslet()},
                {TokenType::Minus,    new PrefixParslet(Precedence::Prefix)},
                {TokenType::LParen,   new GroupParslet()},
                {TokenType::LBracket, new ListParslet()}
        };
        m_infix_parslets = {
                {TokenType::Plus,        new InfixParslet(Precedence::Sum)},
                {TokenType::Minus,       new InfixParslet(Precedence::Sum)},
                {TokenType::LessThan,    new InfixParslet(Precedence::Bitshift)},
                {TokenType::GreaterThan, new InfixParslet(Precedence::Bitshift)},
                {TokenType::Asterisk,    new InfixParslet(Precedence::Product)},
                {TokenType::Slash,       new InfixParslet(Precedence::Product)},
                {TokenType::Percent,       new InfixParslet(Precedence::Product)},
                {TokenType::Exclamation, new PostfixParslet(Precedence::Postfix)},
        };
    }

    Parser::~Parser() {
        // Clean up heap allocated parselets.
        for (auto &entry: m_prefix_parslets) {
            delete entry.second;
        }
        for (auto &entry: m_infix_parslets) {
            delete entry.second;
        }
    }

    std::shared_ptr<ASTNode> Parser::parse_expression(Precedence precedence) {
        Token token = consume();

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
            token = consume();

            auto infix = m_infix_parslets.at(token.type);
            node = infix->parse(*this, token, node);
        }

        return node;
    }

    Precedence Parser::get_precedence() {
        TokenType type = look_ahead().type;
        if (m_infix_parslets.find(type) != m_infix_parslets.end()) {
            return m_infix_parslets.at(type)->get_precedence();
        }
        return Precedence::None;
    }

    bool Parser::matches(const TokenType &expected) {
        Token token = look_ahead();
        if (token.type != expected)
            return false;

        consume();
        return true;
    }

    Token Parser::consume(const TokenType &expected) {
        Token token = look_ahead();
        if (token.type != expected) {
            std::stringstream ss;
            ss << "Expected token type '";
            ss << (int) expected;
            ss << "' but got '";
            ss << token.value;
            ss << "'";
            throw std::runtime_error(ss.str());
        }
        return consume();
    }

    Token Parser::look_ahead(int distance) {
        while (distance >= m_tokens.size()) {
            m_tokens.emplace_back(m_lexer.next());
        }
        return m_tokens.at(distance);
    }

    Token Parser::consume() {
        look_ahead();
        Token token = m_tokens.at(0);
        m_tokens.pop_back();
        return token;
    }

    std::optional<Token> Parser::try_consume(const TokenType &expected) {
        Token token = look_ahead();
        if (token.type != expected)
            return {};
        return consume();
    }

}



