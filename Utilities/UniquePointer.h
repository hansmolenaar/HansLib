#pragma once

#include "Defines.h"

#include <algorithm>
#include <vector>

namespace Utilities
{
template <typename T> std::vector<T *> getPointers(const std::vector<std::unique_ptr<T>> &vec)
{
    std::vector<T *> result(vec.size());
    str::transform(vec, result.begin(), [](const auto &up) { return up.get(); });
    return result;
}

template <typename Tout, typename Tin> std::vector<Tout *> getCastPointers(const std::vector<std::unique_ptr<Tin>> &vec)
{
    std::vector<Tout *> result(vec.size());
    str::transform(vec, result.begin(), [](const auto &up) { return dynamic_cast<Tout *>(up.get()); });
    return result;
}

template <typename T> std::vector<const T *> getCPointers(const std::vector<std::unique_ptr<T>> &vec)
{
    std::vector<const T *> result(vec.size());
    str::transform(vec, result.begin(), [](const auto &up) { return up.get(); });
    return result;
}

} // namespace Utilities
