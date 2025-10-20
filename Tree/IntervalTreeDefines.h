#pragma once

namespace IntervalTree
{
using Level = int;

template <size_t N> constexpr int NumKids = 1 << N;
} // namespace IntervalTree
