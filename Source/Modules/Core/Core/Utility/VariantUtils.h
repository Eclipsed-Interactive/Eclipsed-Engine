#pragma once

namespace Eclipse::Utility
{
    template<class... Ts>
    struct overloaded : Ts...
    {
        using Ts::operator()...;
    };

    template<class... Ts>
    overloaded(Ts...) -> overloaded<Ts...>;
}