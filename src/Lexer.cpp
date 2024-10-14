#include "Lexer.h"
#include <fstream>
#include <iostream>
#include <utility>

namespace Chinstrap {
    Lexer::Lexer(std::string_view source) : m_source(source) {
        m_current = &m_source[0];
    }

    bool Lexer::is_end() {
        return *m_current == '\0';
    }

    Token Lexer::next() {
        skip_whitespace();

        if (std::isalpha(*m_current))
            return lex_identifier();

        if (std::isdigit(*m_current) || (*m_current == '.' && std::isdigit(peek())))
            return lex_number();

        switch (*m_current) {
            case '=':
                return create_token(TokenType::Equals, advance());
            case ',':
                return create_token(TokenType::Comma, advance());
            case ';':
                return create_token(TokenType::Semicolon, advance());
            case '+':
                return create_token(TokenType::Plus, advance());
            case '-':
                return create_token(TokenType::Minus, advance());
            case '/':
                return create_token(TokenType::Slash, advance());
            case '%':
                return create_token(TokenType::Percent, advance());
            case '*':
                return create_token(TokenType::Asterisk, advance());
            case '!':
                return create_token(TokenType::Exclamation, advance());
            case '(':
                return create_token(TokenType::LParen, advance());
            case ')':
                return create_token(TokenType::RParen, advance());
            case '[':
                return create_token(TokenType::LBracket, advance());
            case ']':
                return create_token(TokenType::RBracket, advance());
            case '{':
                return create_token(TokenType::LBrace, advance());
            case '}':
                return create_token(TokenType::RBrace, advance());
            case '<':
                return create_token(TokenType::LessThan, advance());
            case '>':
                return create_token(TokenType::GreaterThan, advance());
            case '|':
                return create_token(TokenType::Pipe, advance());
            case '&':
                return create_token(TokenType::Ampersand, advance());
        }

        return create_token(TokenType::Eof, "");
    }

    char Lexer::advance() {
        return *m_current++;
    }

    char Lexer::peek() {
        return *(m_current + 1);
    }

    bool Lexer::match(char c) {
        return peek() == c;
    }

    void Lexer::skip_whitespace() {
        while (true) {
            switch (*m_current) {
                case ' ':
                case '\r':
                case '\t': {
                    advance();
                    break;
                }
                case '\n': {
                    advance();
                    m_line++;
                    break;
                }
                case '#': {
                    while (!is_end() && peek() != '\n') {
                        advance();
                    }
                    break;
                }
                default:
                    return;
            }
        }
    }

    Token Lexer::create_token(TokenType type, std::string value) const {
        return {type, std::move(value), m_line};
    }

    Token Lexer::create_token(TokenType type, char value) const {
        return {type, value, m_line};
    }

    Token Lexer::lex_number() {
        std::stringstream ss;
        uint32_t dot_count = 0;
        while (!is_end() && (std::isdigit(*m_current) || ((*m_current == '.') && dot_count < 1))) {
            if (*m_current == '.')
                dot_count++;
            ss << advance();
        }
        if (dot_count > 0) {
            return create_token(TokenType::Real, ss.str());
        } else {
            return create_token(TokenType::Integer, ss.str());
        }

    }

    Token Lexer::lex_identifier() {
        std::stringstream ss;
        while (!is_end() && std::isalnum(*m_current))
            ss << advance();

        auto string = ss.str();

        if (string == "if") {
            return create_token(TokenType::KW_if, string);
        } else if (string == "def") {
            return create_token(TokenType::KW_def, string);
        } else if (string == "return") {
            return create_token(TokenType::KW_return, string);
        }
        return create_token(TokenType::Identifier, ss.str());
    }

    std::string Lexer::read_source(const std::string &source_path) {
        std::ifstream file(source_path);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
}
