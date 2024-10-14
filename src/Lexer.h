#ifndef CHINSTRAP_LEXER_H
#define CHINSTRAP_LEXER_H

#include <string>
#include <optional>
#include <sstream>

namespace Chinstrap {
    enum class TokenType {
        LexError = 0,
        Integer,
        Real,
        Identifier,
        Comma,
        Semicolon,
        Plus,
        Minus,
        Asterisk,
        Slash,
        Percent,
        Exclamation,
        Equals,
        LParen,
        RParen,
        LBracket,
        RBracket,
        LBrace,
        RBrace,
        LessThan,
        GreaterThan,
        Pipe,
        Ampersand,
        Eof,
        KW_if,
        KW_def,
        KW_return,
    };

    struct Token {
        TokenType m_type;
        std::string m_value;

        size_t m_line_number;

        Token(TokenType type, std::string value, size_t line_number) : m_type(type), m_value(std::move(value)),
                                                                       m_line_number(line_number) {}

        Token(TokenType type, char value, size_t line_number) : m_type(type), m_line_number(line_number) {
            m_value = std::string(1, value);
        }

        friend std::ostream &operator<<(std::ostream &os, const Token &token) {
            std::stringstream ss;
            ss << "Token Type: " << static_cast<int>(token.m_type);
            ss << " ";
            ss << "Token Value: \"" << token.m_value << "\"";
            os << ss.str();
            return os;
        }
    };


    class Lexer {
    public:
        explicit Lexer(std::string_view source);

//        Lexer(const std::string& source_path);
        bool is_end();

        Token next();

    private:
        static std::string read_source(const std::string &);

        char advance();

        char peek();

        bool match(char c);

        void skip_whitespace();

        [[nodiscard]] Token create_token(TokenType type, std::string value) const;

        [[nodiscard]] Token create_token(TokenType type, char value) const;

        Token lex_number();

        Token lex_identifier();

        size_t m_line{1};
        const char *m_current;
        std::string m_source;
    };
}


#endif //CHINSTRAP_LEXER_H
