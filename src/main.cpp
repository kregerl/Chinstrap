#include <iostream>
#include <utility>
#include "Parser.h"

void interpret(std::shared_ptr<Chinstrap::ASTNode> node) {
    auto variant = Chinstrap::Interpreter::get_value(std::move(node));
    std::visit(Chinstrap::overloaded{
            [](auto &result) { std::cout << result << std::endl; }
    }, variant);
}

void pretty_print(std::shared_ptr<Chinstrap::ASTNode> node) {
    Chinstrap::PrettyPrinter::get_value(std::move(node));
}

int main() {
//    Chinstrap::Parser p("--12 + 1 - 10");
//    Chinstrap::Parser p("(1 + (2 + 2) * 2)");
//    Chinstrap::Parser p("1 + 1");
//    Chinstrap::Parser p("1 + 2 * 2 << 1 + 2 * 2");
//    Chinstrap::Parser p("5!");
    Chinstrap::Parser p("[1, 2, 3, 4, 5]");
//    Chinstrap::Parser p("10 % 2");
    auto node = p.parse_expression();

    interpret(node);
//    pretty_print(node);
}

