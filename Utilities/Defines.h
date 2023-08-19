#pragma once

#include <ranges>
#include <algorithm>
#include <vector>
#include <array>
#include <memory>
#include <map>
#include <numeric>
#include <limits>
#include <cmath>

namespace str = std::ranges;
namespace stv = std::views;

using PointIndex = size_t;
inline constexpr PointIndex PointIndexInvalid = std::numeric_limits<size_t>::max();
using CellIndex = size_t;
inline constexpr CellIndex CellIndexInvalid = std::numeric_limits<size_t>::max();