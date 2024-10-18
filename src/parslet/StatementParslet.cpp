#include "StatementParslet.h"
#include "../Parser.h"

namespace Chinstrap {
    StatementParslet::StatementParslet(Precedence precedence) : Parslet(precedence) {}

    std::shared_ptr<ASTNode> StatementParslet::parse(Parser& parser, const Token& token) {
        
        switch (token.m_type) {
            case TokenType::KW_if: {

                break;
            }

            default:break;
        }
        
    }
}
