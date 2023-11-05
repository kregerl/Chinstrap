#ifndef CHINSTRAP_REPL_H
#define CHINSTRAP_REPL_H

#include "../Visitor.h"

namespace Repl {
    class Cli {
    public:
        Cli() = default;
        void repl();
    private:
        Chinstrap::Interpreter m_interpreter;
        std::string m_expression;
    };
}


#endif //CHINSTRAP_REPL_H
