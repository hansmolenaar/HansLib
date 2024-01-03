#pragma once

#include "IntervalTree.h"
#include "ReferenceShapePolygon.h"
#include "VtkData.h"

#include <vector>
#include <array>

namespace IndexTreeToSimplices2
{
	constexpr int GeometryDimension = 2;
	std::vector<std::array<RatPoint2, ReferenceShapePolygon::TriangleNumCorners>> Create(const IntervalTree::IndexTree<2>& tree);
	std::unique_ptr<Vtk::VtkData> ToVtkData(const std::vector<std::array<RatPoint2, ReferenceShapePolygon::TriangleNumCorners>>& cells);
};