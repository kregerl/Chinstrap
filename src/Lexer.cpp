#include "Lexer.h"
#include <fstream>
#include <iostream>

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
                return Token::create(TokenType::Equals, advance());
            case ',':
                return Token::create(TokenType::Comma, advance());
            case '+':
                return Token::create(TokenType::Plus, advance());
            case '-':
                return Token::create(TokenType::Minus, advance());
            case '/':
                return Token::create(TokenType::Slash, advance());
            case '%':
                return Token::create(TokenType::Percent, advance());
            case '*':
                return Token::create(TokenType::Asterisk, advance());
            case '!':
                return Token::create(TokenType::Exclamation, advance());
            case '(':
                return Token::create(TokenType::LParen, advance());
            case ')':
                return Token::create(TokenType::RParen, advance());
            case '[':
                return Token::create(TokenType::LBracket, advance());
            case ']':
                return Token::create(TokenType::RBracket, advance());
            case '<':
                return Token::create(TokenType::LessThan, advance());
            case '>':
                return Token::create(TokenType::GreaterThan, advance());
            case '|':
                return Token::create(TokenType::Pipe, advance());
            case '&':
                return Token::create(TokenType::Ampersand, advance());
        }

        return Token::create(TokenType::Eof, "");
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

    Token Lexer::lex_number() {
        std::stringstream ss;
        uint32_t dot_count = 0;
        while (!is_end() && (std::isdigit(*m_current) || ((*m_current == '.') && dot_count < 1))) {
            if (*m_current == '.')
                dot_count++;
            ss << advance();
        }
        if (dot_count > 0) {
            return Token::create(TokenType::Real, ss.str());
        } else {
            return Token::create(TokenType::Integer, ss.str());
        }

    }

    Token Lexer::lex_identifier() {
        std::stringstream ss;
        while (!is_end() && std::isalnum(*m_current))
            ss << advance();
        return Token::create(TokenType::Identifier, ss.str());
    }

    std::string Lexer::read_source(const std::string &source_path) {
        std::ifstream file(source_path);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
}
