#pragma once

#include "IntervalTree.h"
#include "ProjectToVtk.h"
#include "TopologyDefines.h"

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
   void cellsToVtkData(MeshGeneration::ProjectToVtk& vtk, const Tetrahedrons& cells);
};