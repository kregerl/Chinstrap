#include <iostream>
#include <termios.h>
#include "Cli.h"
#include "../Parser.h"

#define STDOUT 1

namespace Repl {
    static void prompt() {
        std::cout << "Mash > ";
    }

    static void move_backwards(int n) {
        printf("\033[%dD", n);
    }

    static void move_forwards(int n) {
        printf("\033[%dC", n);
    }

    static void clear_n_chars(int n) {
        printf("\033[%dJ", n);
    }

    [[noreturn]] void Cli::repl() {
        // struct termios term{};
        // tcgetattr(STDOUT, &term);
        // term.c_lflag &= ~ICANON;
        // tcsetattr(STDOUT, TCSANOW, &term);
        // while (true) {
        //     prompt();
        //     uint32_t c;
        //     while ((c = getchar()) != 10) {
        //         if (c == '\x1b' && getchar() == '[') {
        //             char next_char = static_cast<char>(getchar());
        //             if (next_char == 'A') {
        //                 printf("\33[2K\r");
        //                 prompt();
        //                 m_history_index++;
        //                 if (m_history.size() - m_history_index - 1 >= m_history.size()) {
        //                     m_history_index = static_cast<int32_t>(m_history.size()) - 1;
        //                 }
        //                 auto str = m_history.at(m_history.size() - m_history_index - 1);
        //                 printf("%s", str.c_str());
        //                 m_expression.clear();
        //                 m_expression.append(str);
        //             } else if (next_char == 'B') {
        //                 printf("\33[2K\r");
        //                 prompt();
        //                 m_history_index = std::max(m_history_index - 1, 0);
        //                 if (m_history.size() - m_history_index - 1 >= m_history.size()) {
        //                     m_history_index = static_cast<int32_t>(m_history.size()) - 1;
        //                 }
        //                 auto str = m_history.at(m_history.size() - m_history_index - 1);
        //                 printf("%s", str.c_str());
        //                 m_expression.clear();
        //                 m_expression.append(str);
        //             } else if (next_char == 'C') {
        //                 printf("\33[2K\r");
        //                 prompt();
        //                 printf("%s", m_expression.c_str());
        //                 if (m_cursor_position == 1) {
        //                     move_forwards(1);
        //                     m_cursor_position--;
        //                 } else if (m_cursor_position != 0 && m_cursor_position <= m_expression.size()) {
        //                     m_cursor_position--;
        //                 }
        //                 move_backwards(m_cursor_position);
        //             } else if (next_char == 'D') {
        //                 printf("\33[2K\r");
        //                 prompt();
        //                 printf("%s", m_expression.c_str());
        //                 if (m_cursor_position <= m_expression.size() - 1) {
        //                     m_cursor_position++;
        //                 }
        //                 move_backwards(m_cursor_position);
        //             }
        //         } else {
        //             m_expression.push_back(static_cast<char>(c));
        //         }
        //     }
        //     printf("Index: %d\n", m_history_index);
        //     printf("Expression: %s\n", m_expression.c_str());
        //     m_history.emplace_back(m_expression);
        //     m_expression.clear();
        //     m_history_index = -1;
        // }


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
           m_history.emplace_back(m_expression);
       }
    }

    void Cli::move_cursor() {
        if (m_cursor_position > 0 )
            move_backwards(m_cursor_position);



//        if (m_cursor_position < m_expression.size() - 1) {
//            m_cursor_position++;
//        }
    }
}
