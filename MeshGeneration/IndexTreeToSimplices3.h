#pragma once

#include "IntervalTree.h"
#include "TopologyDefines.h"
#include "VtkData.h"

#include <array>
#include <vector>

namespace Vtk
{
   struct Name;
}

namespace IndexTreeToSimplices3
{
   using Tetrahedrons = std::vector<std::array<RatPoint3, Topology::NumNodesOnTetrahedron>>;

   Tetrahedrons Create(const IntervalTree::IndexTree<GeomDim3>& tree);
   std::vector< std::unique_ptr<Vtk::VtkData>> cellsToVtkData(const Tetrahedrons& cells, const std::string& projectName);
};