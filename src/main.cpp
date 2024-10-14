#include <iostream>
#include <utility>
#include <functional>
#include "Parser.h"
#include "repl/Cli.h"

void interpret(std::shared_ptr<Chinstrap::ASTNode> node) {
    auto variant = Chinstrap::Interpreter::get_value(std::move(node));
    std::visit(Chinstrap::overloaded{
                   [](Chinstrap::Noop) {
                   },
                   [](auto &result) { std::cout << result << std::endl; }
               }, variant);
}

void pretty_print(std::shared_ptr<Chinstrap::ASTNode> node) {
    Chinstrap::PrettyPrinter::get_value(std::move(node));
}

int main() {
    // std::variant<Chinstrap::IntegerLiteral, Chinstrap::RealLiteral> var1 = Chinstrap::IntegerLiteral(100);
    // std::variant<Chinstrap::IntegerLiteral, Chinstrap::RealLiteral> var2 = Chinstrap::RealLiteral(10.2);
    // std::visit(Chinstrap::overloaded{
    //     [](Chinstrap::Literal &lhs, Chinstrap::Literal &rhs) {
    //         auto x = lhs / rhs;
    //         std::cout << x.get_value() << std::endl;
    //     }
    // }, var1, var2);
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
