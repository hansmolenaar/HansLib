#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <ranges>
#include <span>
#include <vector>

#include "MyException.h"

namespace str = std::ranges;
namespace stv = std::views;

using PointIndex = size_t;
inline constexpr PointIndex PointIndexInvalid = std::numeric_limits<size_t>::max();
using CellIndex = size_t;
inline constexpr CellIndex CellIndexInvalid = std::numeric_limits<size_t>::max();

inline constexpr unsigned int GeomDim1 = 1;
inline constexpr unsigned int GeomDim2 = 2;
inline constexpr unsigned int GeomDim3 = 3;