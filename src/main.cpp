#include <iostream>
#include "Parser.h"

void interpret(std::shared_ptr<Chinstrap::ASTNode> node) {
    auto interpreter = Chinstrap::Interpreter();
    std::cout << node->accept(interpreter) << std::endl;
}

void pretty_print(std::shared_ptr<Chinstrap::ASTNode> node) {
    int indent_amount = 0;
    auto pp = Chinstrap::PrettyPrinter();
    node->accept_print(pp, &indent_amount);
}

int main() {
//    Chinstrap::Parser p("--12 + 1 - 10");
//    Chinstrap::Parser p("(1 + (2 + 2) * 2)");
//    Chinstrap::Parser p("1 + 1");
//    Chinstrap::Parser p("1 + 2 * 2 << 1 + 2 * 2");
    Chinstrap::Parser p("5!");
    auto node = p.parse_expression();
    pretty_print(node);
}

