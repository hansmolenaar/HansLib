#pragma once

#include "IntervalTree.h"
#include "ProjectToVtk.h"
#include "TopologyDefines.h"

#include <array>
#include <set>
#include <vector>

namespace Vtk
{
   struct Name;
}

namespace IndexTreeToSimplices3
{
   using Tetrahedrons = std::vector<std::array<RatPoint3, Topology::NumNodesOnTetrahedron>>;
   using TreeEdge = std::array<RatPoint3, Topology::NumNodesOnEdge>;

   Tetrahedrons Create(const IntervalTree::IndexTree<GeomDim3>& tree);
   void cellsToVtkData(MeshGeneration::ProjectToVtk& vtk, const Tetrahedrons& cells);

   std::array<RatPoint3, Topology::NumNodesOnCube> getCubeFromIndex(const IntervalTree::Index<GeomDim3>& index);
   std::array<TreeEdge, Topology::NumEdgesOnCube> getCubeEdgesFromIndex(const IntervalTree::Index<GeomDim3>& index);
   std::set<IndexTreeToSimplices3::TreeEdge> getRefinedEdges(const IntervalTree::IndexTree<GeomDim3>& tree);
};