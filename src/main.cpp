#include <iostream>
#include <utility>
#include "Parser.h"

void interpret(std::shared_ptr<Chinstrap::ASTNode> node) {
    std::cout << Chinstrap::Interpreter::get_value(std::move(node)) << std::endl;
}

void pretty_print(std::shared_ptr<Chinstrap::ASTNode> node) {
    Chinstrap::PrettyPrinter::get_value(std::move(node));
}

int main() {
//    Chinstrap::Parser p("--12 + 1 - 10");
//    Chinstrap::Parser p("(1 + (2 + 2) * 2)");
//    Chinstrap::Parser p("1 + 1");
    Chinstrap::Parser p("1 + 2 * 2 << 1 + 2 * 2");
//    Chinstrap::Parser p("5!");
    auto node = p.parse_expression();
    pretty_print(node);
}

