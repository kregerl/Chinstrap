#ifndef CHINSTRAP_PARSER_H
#define CHINSTRAP_PARSER_H

#include <string_view>
#include <unordered_map>
#include <stack>
#include "Lexer.h"
#include "ASTNode.h"
#include "parslet/PrefixParslet.h"
#include "parslet/InfixParslet.h"

namespace Chinstrap {

    class Parser {
    public:
        explicit Parser(std::string_view source);

        ~Parser();

        std::shared_ptr<ASTNode> parse_expression(Precedence precedence = Precedence::None);

        bool matches(const TokenType &expected);

        Token consume(const TokenType &expected);

        Token consume();
    private:

        Token look_ahead(int distance = 0);

        Precedence get_precedence();

        Lexer m_lexer;
        std::vector<Token> m_tokens;
        std::unordered_map<TokenType, PrefixParslet *> m_prefix_parslets;
        std::unordered_map<TokenType, InfixParslet *> m_infix_parslets;
    };
}


#endif //CHINSTRAP_PARSER_H
