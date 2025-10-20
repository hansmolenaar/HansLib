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
using Tetrahedrons = std::vector<std::array<RatPoint3, Topology::NumCornersOnTetrahedron>>;

struct TreeEdge
{
    TreeEdge() = default;
    TreeEdge(const RatPoint3 &p0, const RatPoint3 p1) : point0(std::min(p0, p1)), point1(std::max(p0, p1))
    {
    }
    auto operator<=>(const TreeEdge &te) const = default;
    RatPoint3 point0;
    RatPoint3 point1;
};

Tetrahedrons Create(const IntervalTree::IndexTree<GeomDim3> &tree);
void cellsToVtkData(MeshGeneration::ProjectToVtk &vtk, const Tetrahedrons &cells);

std::array<RatPoint3, Topology::NumCornersOnCube> getCubeFromIndex(const IntervalTree::Index<GeomDim3> &index);
std::array<TreeEdge, Topology::NumEdgesOnCube> getCubeEdgesFromIndex(const IntervalTree::Index<GeomDim3> &index);
std::set<TreeEdge> getRefinedEdges(const IntervalTree::IndexTree<GeomDim3> &tree);
}; // namespace IndexTreeToSimplices3