#include "Parslet.h"

namespace Chinstrap {
    Precedence Parslet::get_precedence() {
        return m_precedence;
    }

    Parslet::Parslet(Precedence precedence) : m_precedence(precedence) {}
}

