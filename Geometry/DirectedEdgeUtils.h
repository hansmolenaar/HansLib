#pragma once

#include "DirectedEdge.h"
#include "DirectedEdgeIntersection.h"
#include "IGeometryPredicate.h"

#include <optional>

namespace Geometry
{
namespace DirectedEdgeUtils
{
// throws if edge is degenerate target can be degenerate
std::optional<DirectedEdgeIntersection<double, 2>> intersectWith2(
    const DirectedEdge2 &edge, const DirectedEdge2 &target, const IGeometryPredicate<double, GeomDim2> &predicate);
} // namespace DirectedEdgeUtils

} // namespace Geometry