#pragma once

#include <ostream>

namespace StreamUtils
{
template <typename C>
std::ostream &insertList(std::ostream &os, const C &container, std::string open = "(", std::string sep = ", ",
                         std::string close = ")")
{
    os << open;
    bool first = true;
    for (auto elm : container)
    {
        if (!first)
        {
            os << sep;
        }
        first = false;
        os << elm;
    }
    os << close;
    return os;
}
} // namespace StreamUtils