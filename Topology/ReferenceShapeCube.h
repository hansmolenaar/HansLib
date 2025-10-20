#pragma once

#include "IReferenceShape.h"
#include "TetrahedronNodesOriented.h"
#include "TopologicalAdjacencies.h"

namespace Topology
{
class ReferenceShapeCube : public IReferenceShape
{
  public:
    static const ReferenceShapeCube &getInstance();
    const ITopologicalAdjacencies &getAdjacencies() const override;

    static constexpr size_t numTetsInStandardSplit = 6;
    const std::array<TetrahedronNodesOriented, numTetsInStandardSplit> &getStandardSplit() const;

  private:
    ReferenceShapeCube();
    std::unique_ptr<TopologicalAdjacencies> m_adjacencies;
};
} // namespace Topology