#include <iostream>
#include "Repl.h"
#include "../Parser.h"

namespace Repl {
    void Cli::repl() {
        while (true) {
            std::cout << "Mash > ";
            getline(std::cin, m_expression);
            if (m_expression == "exit") {
                return;
            } else if (m_expression == "clear") {
                system("clear");
            } else {
                Chinstrap::Parser parser(m_expression);
                auto node = parser.parse_expression();
                auto variant = Chinstrap::Interpreter::get_value(node);
                std::visit(Chinstrap::overloaded{
                        [](Chinstrap::Noop &) {},
                        [](auto &result) { std::cout << result << std::endl; }
                }, variant);
            }
        }
    }
}
