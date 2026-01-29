#pragma once

#include "Defines.h"
#include "MyException.h"
#include <optional>
#include <sstream>

namespace Utilities
{
template <typename C> std::optional<typename C::value_type> firstDuplicate(const C &values)
{
    if (values.size() < 2)
        return {};
    if (values.size() == 2)
    {
        if (values[0] == values[1])
            return {values[0]};
        return {};
    }
    if (values.size() == 3)
    {
        if (values[0] == values[1] || values[0] == values[2])
            return {values[0]};
        if (values[1] == values[2])
            return {values[1]};
        return {};
    }
    if (values.size() == 4)
    {
        if (values[0] == values[1] || values[0] == values[2] || values[0] == values[3])
            return {values[0]};
        if (values[1] == values[2] || values[1] == values[3])
            return {values[1]};
        if (values[2] == values[3])
            return {values[2]};
        return {};
    }

    std::vector<typename C::value_type> sorted(values.begin(), values.end());
    std::sort(sorted.begin(), sorted.end());
    const auto found = str::adjacent_find(sorted);
    if (found != sorted.end())
    {
        return *found;
    }
    return {};
}

template <typename C> void throwOnDuplicate(const C &values)
{
    const auto found = firstDuplicate(values);
    if (found)
    {
        std::ostringstream msg;
        msg << "Container has duplicate: " << *found;
        throw MyException(msg.str());
    }
}
} // namespace Utilities
