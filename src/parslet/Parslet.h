#ifndef CHINSTRAP_PARSLET_H
#define CHINSTRAP_PARSLET_H

#include "../Precedence.h"
#include "../ASTNode.h"

namespace Chinstrap {
    class Parser;

    class Parslet {
    public:
        explicit Parslet(Precedence precedence);

        Precedence get_precedence();

    private:
        Precedence m_precedence;
    };
}


#endif //CHINSTRAP_PARSLET_H
