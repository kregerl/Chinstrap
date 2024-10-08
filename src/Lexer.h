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
    };

    struct Token {
        TokenType type;
        std::string value;

        static Token create(TokenType type, std::string value) {
            return Token{.type=type, .value=std::move(value)};
        }

        static Token create(TokenType type, char value) {
            return Token{.type=type, .value=std::string(1, value)};
        }

        friend std::ostream& operator<<(std::ostream& os, const Token& token) {
            std::stringstream ss;
            ss << "Token Type: " << static_cast<int>(token.type);
            ss << " ";
            ss << "Token Value: \"" << token.value << "\"";
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
        static std::string read_source(const std::string&);

        char advance();

        char peek();

        bool match(char c);

        void skip_whitespace();

        Token lex_number();

        Token lex_identifier();

        size_t m_line{1};
        const char* m_current;
        std::string m_source;
    };
}


#endif //CHINSTRAP_LEXER_H
