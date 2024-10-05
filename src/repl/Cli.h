#ifndef CHINSTRAP_CLI_H
#define CHINSTRAP_CLI_H

#include "../Visitor.h"

namespace Repl {
    class Cli {
    public:
        Cli() = default;
        [[noreturn]] void repl();
        void move_cursor();

    private:
        std::vector<std::string> m_history;
        int32_t m_cursor_position = 0;
        int32_t m_history_index = -1;

        Chinstrap::Interpreter m_interpreter;
        std::string m_expression;
    };
}


#endif //CHINSTRAP_CLI_H
