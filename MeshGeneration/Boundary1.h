#pragma once

#include "CycleNodes.h"
#include "EdgeNodesSorted.h"
#include "PathNodes.h"
#include "TrianglesNodes.h"

#include <set>

namespace MeshGeneration
{
class Boundary1
{
  public:
    Boundary1() = default;

    static Boundary1 createFromSubSet(const std::vector<Topology::NodeIndex> &activeNodes,
                                      const TrianglesNodes &trianglesNodes);
    static Boundary1 createFromBoundaryEdges(const TrianglesNodes &trianglesNodes);
    const std::vector<Topology::NodeIndex> &getSingletons() const;
    const std::vector<Topology::PathNodes> &getPaths() const;
    const std::vector<Topology::CycleNodes> &getCycles() const;
    bool empty() const;
    std::set<Topology::EdgeNodesSorted> getEdges() const;

  private:
    explicit Boundary1(const std::vector<Topology::EdgeNodesSorted> &edgeSet,
                       std::vector<Topology::NodeIndex> xNodes = {});

    std::vector<Topology::NodeIndex> m_singletons;
    std::vector<Topology::PathNodes> m_paths;
    std::vector<Topology::CycleNodes> m_cycles;
};

} // namespace MeshGeneration