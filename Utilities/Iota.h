#pragma once

#include "Defines.h"

namespace Iota
{
template <typename T> inline std::vector<T> GenerateVector(T siz, T from = 0)
{
    std::vector<T> result(siz);
    std::iota(result.begin(), result.end(), from);
    return result;
}

inline std::vector<int> GenerateIntVector(size_t siz, int from = 0)
{
    return GenerateVector<int>(siz, from);
}

template <typename T> inline auto GetRange(T siz)
{
    return stv::iota(T{0}, siz);
}

template <typename T> inline auto GetRangeFromTo(T lbInclusive, T ubExclusive)
{
    return stv::iota(lbInclusive, std::max(lbInclusive, ubExclusive));
}

} // namespace Iota
