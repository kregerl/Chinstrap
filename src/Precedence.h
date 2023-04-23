#ifndef CHINSTRAP_PRECEDENCE_H
#define CHINSTRAP_PRECEDENCE_H

namespace Chinstrap {
    enum class Precedence {
        None,
        Assignment,
        Conditional,
        Sum,
        Product,
        Exponent,
        Prefix,
        Postfix,
        Call
    };
}
#endif //CHINSTRAP_PRECEDENCE_H
