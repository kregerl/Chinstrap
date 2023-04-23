#include <iostream>
#include "Parser.h"

int main() {
//    Chinstrap::Parser p("--12 + 1 - 10");
    Chinstrap::Parser p("(1 + (2 + 2) * 2)");
//    Chinstrap::Parser p("(1 + 2");
    auto node = p.parse_expression();
    auto interpreter = Chinstrap::Interpreter();
    std::cout << node->accept(interpreter) << std::endl;
}
