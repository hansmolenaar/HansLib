#pragma once

#include "Defines.h"

#include <algorithm>
#include <vector>

namespace Utilities
{
template <typename C> void UniqueSorted(std::vector<C> &vec)
{
    if (vec.size() < 2)
    {
        return;
    }
    str::sort(vec);
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}

} // namespace Utilities
