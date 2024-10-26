#include <iostream>
#include <utility>
#include <functional>
#include "Parser.h"
#include "repl/Cli.h"

void interpret(std::shared_ptr<Chinstrap::ASTNode> node) {
    auto variant = Chinstrap::Interpreter::get_value(std::move(node));
    std::visit(Chinstrap::overloaded{
            [](std::monostate) {},
            [](auto &result) { std::cout << result << std::endl; }
    }, variant);
}

void pretty_print(std::shared_ptr<Chinstrap::ASTNode> node) {
    Chinstrap::PrettyPrinter::get_value(std::move(node));
}

int main() {
    Repl::Cli cli;
    cli.repl();
    //    Chinstrap::Parser p("--12 + 1 - 10");
    //    Chinstrap::Parser p("(1 + (2 + 2) * 2)");
    //    Chinstrap::Parser p("1 + 1");
    //    Chinstrap::Parser p("1 + 2 * 2 << 1 + 2 * 2");
    //    Chinstrap::Parser p("5!");
    //    Chinstrap::Parser p("[1, 2, 3, 4, 5]");
    //    Chinstrap::Parser p("10 % 2");
    //    Chinstrap::Parser p("x = 10");
    //    auto node = p.parse_expression();
//     Chinstrap::Parser p("def test(x) {x+1}");
//     auto node = p.parse_expression();
//     interpret(node);
    //    pretty_print(node);
}


