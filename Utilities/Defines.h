#pragma once

#include <ranges>
#include <algorithm>
#include <vector>
#include <array>
#include <memory>
#include <map>
#include <numeric>
#include <limits>

namespace str = std::ranges;

using PointIndex = size_t;
inline constexpr PointIndex PointIndexInvalied = std::numeric_limits<size_t>::max();
using CellIndex = size_t;
inline constexpr CellIndex CellIndexInvalied = std::numeric_limits<size_t>::max();