#ifndef CHINSTRAP_RETURNABLE_H
#define CHINSTRAP_RETURNABLE_H

#include "types/Literal.h"
#include "types/Collection.h"

namespace Chinstrap {
    using Returnable = std::variant<std::monostate, int64_t, double, bool, Collection>;
}

#endif