#pragma once
#include "Defines.h"

#include <limits>

namespace Topology
{
using TopologyDimension = unsigned int;
inline constexpr TopologyDimension Corner = 0;
inline constexpr TopologyDimension Edge = 1;
inline constexpr TopologyDimension Face = 2;
inline constexpr TopologyDimension Volume = 3;

inline constexpr size_t NumCornersOnEdge = 2;
inline constexpr size_t NumCornersOnTriangle = 3;
inline constexpr size_t NumCornersOnSquare = 4;
inline constexpr size_t NumCornersOnTetrahedron = 4;
inline constexpr size_t NumCornersOnCube = 8;

inline constexpr size_t NumEdgesOnTriangle = 3;
inline constexpr size_t NumEdgesOnSquare = 4;
inline constexpr size_t NumEdgesOnTetrahedron = 6;
inline constexpr size_t NumEdgesOnCube = 12;

inline constexpr size_t NumFacesOnTetrahedron = 4;
inline constexpr size_t NumFacesOnCube = 6;

using NodeIndex = PointIndex;
inline constexpr NodeIndex NodeIndexInvalid = PointIndexInvalid;
} // namespace Topology