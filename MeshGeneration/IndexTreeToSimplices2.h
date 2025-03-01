#pragma once

#include "IntervalTree.h"
#include "ReferenceShapePolygon.h"
#include "VtkData.h"

#include <vector>
#include <array>

namespace Vtk
{
   struct Name;
}

namespace IndexTreeToSimplices2
{
   using Triangles = std::vector<std::array<RatPoint2, ReferenceShapePolygon::TriangleNumCorners>>;

   constexpr int GeometryDimension = 2;
   Triangles Create(const IntervalTree::IndexTree<2>& tree);
   std::unique_ptr<Vtk::VtkData> ToVtkData(const Triangles& cells, const Vtk::Name& name);
};