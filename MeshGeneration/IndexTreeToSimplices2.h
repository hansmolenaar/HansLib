#pragma once

#include "IntervalTree.h"
#include "ReferenceShapePolygon.h"
#include "VtkData.h"

#include <array>
#include <vector>

namespace Vtk
{
struct Name;
}

namespace MeshGeneration
{
class ProjectToVtk;
}

namespace IndexTreeToSimplices2
{
using Triangles = std::vector<std::array<RatPoint2, Topology::ReferenceShapePolygon::TriangleNumCorners>>;

constexpr int GeometryDimension = 2;
Triangles Create(const IntervalTree::IndexTree<2> &tree);
void toVtkData(MeshGeneration::ProjectToVtk &p2v, const Triangles &cells);
}; // namespace IndexTreeToSimplices2