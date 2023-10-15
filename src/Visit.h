#ifndef CHINSTRAP_VISIT_H
#define CHINSTRAP_VISIT_H

namespace Chinstrap {

    template<class... Ts>
    struct overloaded : Ts ... {
        using Ts::operator()...;
    };

    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
}

#endif //CHINSTRAP_VISIT_H
