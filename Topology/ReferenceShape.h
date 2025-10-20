#pragma once

#include "IReferenceShape.h"
#include <memory>

namespace Topology
{
class ReferenceShape : public IReferenceShape
{
  public:
    ReferenceShape(std::unique_ptr<ITopologicalAdjacencies> &&);
    const ITopologicalAdjacencies &getAdjacencies() const override;

  private:
    std::unique_ptr<ITopologicalAdjacencies> m_adjacencies;
};
} // namespace Topology
