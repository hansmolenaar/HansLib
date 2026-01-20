#pragma once

#include "Defines.h"

namespace Iota
{
template <typename T> inline std::vector<T> GenerateVector(size_t siz, T from = 0)
{
    std::vector<T> result(siz);
    std::iota(result.begin(), result.end(), from);
    return result;
}

inline std::vector<int> GenerateIntVector(size_t siz, int from = 0)
{
    return GenerateVector<int>(siz, from);
}

template <typename T> inline auto GetRange(T siz, T from = 0)
{
    return stv::iota(from, siz);
}
} // namespace Iota
