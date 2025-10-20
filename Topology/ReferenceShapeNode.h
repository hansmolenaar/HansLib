#pragma once

#include "IReferenceShape.h"

namespace Topology
{
class ReferenceShapeNode : public IReferenceShape
{
  public:
    const ITopologicalAdjacencies &getAdjacencies() const override;
};
} // namespace Topology