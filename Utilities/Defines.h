#pragma once

#include <cmath>
#include <limits>

using PointIndex = size_t;
inline constexpr PointIndex PointIndexInvalid = std::numeric_limits<size_t>::max();
using CellIndex = size_t;
inline constexpr CellIndex CellIndexInvalid = std::numeric_limits<size_t>::max();

inline constexpr size_t GeomDim1 = 1;
inline constexpr size_t GeomDim2 = 2;
inline constexpr size_t GeomDim3 = 3;
